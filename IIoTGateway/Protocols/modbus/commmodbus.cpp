#include "commmodbus.h"

#include <QEventLoop>
#include <QFile>

CommModbus::CommModbus()
{
    m_modbusClient = nullptr;
    m_polling = nullptr;
}

CommModbus::~CommModbus()
{
    if (m_modbusClient)
    {
        m_modbusClient->disconnect();

        delete m_modbusClient;
    }

    if (m_polling)
    {
        m_polling->disconnect();
        m_polling->stop();

        delete m_polling;
    }
}

void
CommModbus::disconnectComm()
{
    m_modbusClient->disconnectDevice();
}

bool
CommModbus::isconnected()
{
    return (m_modbusClient->state() == QModbusDevice::ConnectedState);
}

ModbusClientInterface*
CommModbus::modbusClient()
{
    return m_modbusClient;
}

void
CommModbus::setModbusClient(ModbusClientInterface *client)
{
    m_modbusClient = client;
}

void
CommModbus::incoming(QByteArray data)
{

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
    m_polling->stop();

    qDebug() << "-------------------- Start --------------------";

    ModbusJsonParser::RequestIterator requests(m_readRequest);

    while (requests.hasNext())
    {
        requests.next();

        quint8 address = requests.key();
        QModbusDataUnit unit = requests.value();

        if (auto *reply = m_modbusClient->sendReadRequest(unit, address))
        {
            QEventLoop loop;
            connect(reply, &QModbusReply::finished, &loop, &QEventLoop::quit);

            loop.exec();
            readReady(reply);
        }
        else
        {
            emit error(QString("Read error: %1").arg(m_modbusClient->errorString()).toUtf8());
        }
    }

    qDebug() << "-------------------- End --------------------";

    m_polling->start();
}

void
CommModbus::writeRegisters()
{
    quint8 address = 240;
    static qint16 data = 1;

    QModbusDataUnit writeUnit = QModbusDataUnit(); //writeRequest();
    QModbusDataUnit::RegisterType table = writeUnit.registerType();

    for (int i = 0, total = int(writeUnit.valueCount()); i < total; ++i)
    {
        if (table == QModbusDataUnit::Coils)
        {
            // writeUnit.setValue(i, m_writeModel.m_coils[i + writeUnit.startAddress()]);
        }
        else
        {
            // writeUnit.setValue(i, m_writeModel.m_holdingRegisters[i + writeUnit.startAddress()]);
            data = data == 1 ? 0 : 1;
            writeUnit.setValue(i, data);
        }
    }

    if (auto *reply = m_modbusClient->sendWriteRequest(writeUnit, address))
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

                QTimer::singleShot(5000, this, &CommModbus::writeRegisters);
            });
        }
        else
        {
            // broadcast replies return immediately
            reply->deleteLater();

            QTimer::singleShot(5000, this, &CommModbus::writeRegisters);
        }
    }
    else
    {
        qDebug() << tr("Write error: ") + m_modbusClient->errorString();
    }
}

void
CommModbus::readReady(QModbusReply *reply)
{
    if (!reply)
    {
        return;
    }

    if (reply->error() == QModbusDevice::NoError)
    {
        const QModbusDataUnit unit = reply->result();

        for (int i = 0, total = int(unit.valueCount()); i < total; ++i)
        {
            // const QString entry = tr("Address: %1, Value: %2").arg(unit.startAddress() + i)
            // .arg(QString::number(unit.value(i), unit.registerType() <= QModbusDataUnit::Coils ? 10 : 16));

            const QString entry = tr("Address: %1, Value: %2").arg(unit.startAddress() + i).arg(QString::number(unit.value(i)));

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
}

void
CommModbus::initPolling()
{
    if (m_polling)
    {
        m_readRequest = loadReadRequestSettings();

        connect(m_polling, &QTimer::timeout, this, &CommModbus::readRegisters);
        m_polling->start();
    }
}

ModbusJsonParser::Request
CommModbus::loadReadRequestSettings()
{
    QFile file("read.json");

    if (!file.open(QIODevice::ReadOnly))
    {
        emit error("Failed opening read.json");

        return ModbusJsonParser::Request();
    }

    ModbusJsonParser parser(file.readAll());

    file.close();

    return parser.readRequest();
}
