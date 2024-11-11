#include "commmodbus.h"
#include "commfactory.h"

#include <QTimer>

int CommModbus::m_typeId = CommFactory::registerInterface<CommModbus*>("MODBUS_RTU");

CommModbus::CommModbus()
{
    // m_modbusDevice = new QModbusRtuSerialClient(this);
    m_writeModel.setStartAddress(1);
    m_writeModel.setNumberOfValues("1");

}

CommModbus::~CommModbus()
{
    m_modbusDevice.disconnectDevice();
}

void
CommModbus::connectComm()
{
    if (m_modbusDevice.state() != QModbusDevice::ConnectedState)
    {
        QVariant port = qgetenv("MODBUS_RTU_PORT");
        QVariant parity = qgetenv("MODBUS_RTU_PARITY");
        QVariant baudRate = qgetenv("MODBUS_RTU_BAUDRATE");
        QVariant dataBits = qgetenv("MODBUS_RTU_DATABITS");
        QVariant stopBits = qgetenv("MODBUS_RTU_STOPBITS");

        bool ok = false;;

        int timeout = qEnvironmentVariableIntValue("MODBUS_RTU_TIMEOUT", &ok);

        timeout = !ok ? 1000 : timeout;

        int retries = qEnvironmentVariableIntValue("MODBUS_RTU_RETRIES", &ok);

        retries = !ok ? 3 : retries;

        m_modbusDevice.setConnectionParameter(QModbusDevice::SerialPortNameParameter, port);
        m_modbusDevice.setConnectionParameter(QModbusDevice::SerialParityParameter, parity);
        m_modbusDevice.setConnectionParameter(QModbusDevice::SerialBaudRateParameter, baudRate);
        m_modbusDevice.setConnectionParameter(QModbusDevice::SerialDataBitsParameter, dataBits);
        m_modbusDevice.setConnectionParameter(QModbusDevice::SerialStopBitsParameter, stopBits);

        m_modbusDevice.setTimeout(timeout);
        m_modbusDevice.setNumberOfRetries(retries);

        connect(&m_modbusDevice, &QModbusClient::stateChanged, this, &CommModbus::stateChanged);

        if (!m_modbusDevice.connectDevice())
        {
            emit error(QString("Connect failed: %1").arg(m_modbusDevice.errorString()).toUtf8());
        }
        else
        {
            QTimer::singleShot(1000, this, &CommModbus::readRegisters);
            QTimer::singleShot(500, this, &CommModbus::writeRegisters);
        }
    }
}

void
CommModbus::disconnectComm()
{
    m_modbusDevice.disconnectDevice();
}

bool
CommModbus::isconnected()
{
    return (m_modbusDevice.state() == QModbusDevice::ConnectedState);
}

void
CommModbus::incoming(QByteArray data)
{

}

QModbusDataUnit
CommModbus::readRequest() const
{
    const auto table = QModbusDataUnit::HoldingRegisters;

    int startAddress = 0;
    Q_ASSERT(startAddress >= 0 && startAddress < 10);

    quint16 readSize = 10;

    // do not go beyond 10 entries
    quint16 numberOfEntries = qMin(readSize, quint16(10 - startAddress));
    return QModbusDataUnit(table, startAddress, numberOfEntries);
}

QModbusDataUnit
CommModbus::writeRequest() const
{
    const auto table = QModbusDataUnit::HoldingRegisters;

    int startAddress = 1;
    Q_ASSERT(startAddress >= 0 && startAddress < 10);

    quint16 readSize = 1;

    // m_writeModel.m_holdingRegisters
    // do not go beyond 10 entries
    quint16 numberOfEntries = qMin(readSize, quint16(10 - startAddress));
    return QModbusDataUnit(table, startAddress, numberOfEntries);
}

void
CommModbus::stateChanged(QModbusDevice::State state)
{
    if (state == QModbusDevice::UnconnectedState)
    {
        emit disconnected();
    }
    else if (state == QModbusDevice::ConnectedState)
    {
        emit connected();
    }
}

void
CommModbus::readRegisters()
{
    quint8 address = 240;

    if (auto *reply = m_modbusDevice.sendReadRequest(readRequest(), address))
    {
        while (!reply->isFinished());

        readReady(reply);

        // if (!reply->isFinished())
        //     connect(reply, &QModbusReply::finished, this, &CommModbus::readReady);
        // else
        //     delete reply; // broadcast replies return immediately
    }
    else
    {
        emit error(QString("Read error: %1").arg(m_modbusDevice.errorString()).toUtf8());
    }
}

void
CommModbus::writeRegisters()
{
    quint8 address = 240;
    static qint16 data = 1;

    QModbusDataUnit writeUnit = writeRequest();
    QModbusDataUnit::RegisterType table = writeUnit.registerType();

    for (int i = 0, total = int(writeUnit.valueCount()); i < total; ++i)
    {
        if (table == QModbusDataUnit::Coils)
        {
            writeUnit.setValue(i, m_writeModel.m_coils[i + writeUnit.startAddress()]);
        }
        else
        {
            // writeUnit.setValue(i, m_writeModel.m_holdingRegisters[i + writeUnit.startAddress()]);
            data = data == 1 ? 0 : 1;
            writeUnit.setValue(i, data);
        }
    }

    if (auto *reply = m_modbusDevice.sendWriteRequest(writeUnit, address))
    {
        if (!reply->isFinished())
        {
            connect(reply, &QModbusReply::finished, this, [this, reply](){

                if (reply->error() == QModbusDevice::ProtocolError)
                {
                    qDebug() <<tr("Write response error: %1 (Mobus exception: 0x%2)")
                    .arg(reply->errorString()).arg(reply->rawResult().exceptionCode(), -1, 16);
                }
                else if (reply->error() != QModbusDevice::NoError)
                {
                    qDebug() << tr("Write response error: %1 (code: 0x%2)").
                                arg(reply->errorString()).arg(reply->error(), -1, 16);
                }

                reply->deleteLater();

                QTimer::singleShot(500, this, &CommModbus::writeRegisters);
            });
        }
        else
        {
            // broadcast replies return immediately
            reply->deleteLater();

            QTimer::singleShot(500, this, &CommModbus::writeRegisters);
        }
    }
    else
    {
        qDebug() << tr("Write error: ") + m_modbusDevice.errorString();
    }
}

void
CommModbus::readReady(QModbusReply *reply)
{
    // auto reply = qobject_cast<QModbusReply*>(sender());

    if (!reply)
    {
        return;
    }

    if (reply->error() == QModbusDevice::NoError)
    {
        const QModbusDataUnit unit = reply->result();

        for (int i = 0, total = int(unit.valueCount()); i < total; ++i)
        {
            const QString entry = tr("Address: %1, Value: %2").arg(unit.startAddress() + i)
            .arg(QString::number(unit.value(i),
                                 unit.registerType() <= QModbusDataUnit::Coils ? 10 : 16));

            qDebug() << entry;
        }
    }
    else if (reply->error() == QModbusDevice::ProtocolError)
    {
        qDebug() << tr("Read response error: %1 (Mobus exception: 0x%2)").
                    arg(reply->errorString()).
                    arg(reply->rawResult().exceptionCode(), -1, 16);
    }
    else
    {
        qDebug() << tr("Read response error: %1 (code: 0x%2)").
                    arg(reply->errorString()).
                    arg(reply->error(), -1, 16);
    }

    reply->deleteLater();

    QTimer::singleShot(1000, this, &CommModbus::readRegisters);
}
