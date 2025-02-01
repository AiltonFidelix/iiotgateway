#include "commmodbus.h"

#include <QCoreApplication>
#include <QEventLoop>
#include <QFile>
#include <QJsonArray>
#include <QJsonObject>

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

    if (ispolling())
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
    qDebug() << "Incoming";
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
CommModbus::readRegisters(const ModbusJsonParser::Request &request)
{
#warning // TODO: Remover QElapsedTimer later
    QElapsedTimer time;
    time.start();

    auto json = QJsonObject();

    const auto addresses = ModbusJsonParser::sortedAddress(request);

    for (const auto &address : addresses)
    {
        auto registers = Registers();
        auto units = request.value(address);
        ModbusJsonParser::sortRequestUnits(units);

        for (const auto &unit : std::as_const(units))
        {
            if (auto *reply = m_modbusClient->sendReadRequest(unit, address))
            {
                QEventLoop loop;
                connect(reply, &QModbusReply::finished, &loop, &QEventLoop::quit);

                loop.exec();
                registers.insert(readReady(reply));
            }
            else
            {
                emit error(QString("Read error: %1").arg(m_modbusClient->errorString()).toUtf8());
            }
        }

        json.insert(QString::number(address), registersToJsonArray(registers));
    }

    emit outgoing(QJsonDocument(json).toJson(QJsonDocument::Compact));

    qDebug() << "readRegisters took" << time.elapsed() << "ms";
}

void
CommModbus::writeRegisters(const ModbusJsonParser::Request &request)
{
    quint8 address = 240;
    static qint16 data = 1;

    auto writeUnit = QModbusDataUnit(); //writeRequest();
    auto table = writeUnit.registerType();

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

                // QTimer::singleShot(5000, this, &CommModbus::writeRegisters);
            });
        }
        else
        {
            // broadcast replies return immediately
            reply->deleteLater();

            // QTimer::singleShot(5000, this, &CommModbus::writeRegisters);
        }
    }
    else
    {
        qDebug() << tr("Write error: ") + m_modbusClient->errorString();
    }
}

CommModbus::Registers
CommModbus::readReady(QModbusReply *reply)
{
    auto registers = Registers();

    if (!reply)
    {
        return registers;
    }

    if (reply->error() == QModbusDevice::NoError)
    {
        const auto unit = reply->result();

        for (int i = 0, total = int(unit.valueCount()); i < total; ++i)
        {
            registers.insert(unit.startAddress() + i, unit.value(i));
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

    return registers;
}

bool
CommModbus::ispolling()
{
    return !(m_polling == nullptr);
}

void
CommModbus::initPolling()
{
    if (!ispolling())
    {
        m_readRequest = loadReadRequestSettings();

        connect(m_polling, &QTimer::timeout, this, &CommModbus::pollingCallback);

        m_polling->start();
    }
}

void
CommModbus::pollingCallback()
{
    m_polling->stop();

    readRegisters(m_readRequest);

    m_polling->start();
}

ModbusJsonParser::Request
CommModbus::loadReadRequestSettings()
{
    QFile file(QString("%1/read.json").arg(QCoreApplication::applicationDirPath()));

    if (!file.open(QIODevice::ReadOnly))
    {
        emit error("Failed opening read.json");

        return ModbusJsonParser::Request();
    }

    ModbusJsonParser parser(file.readAll());

    file.close();

    return parser.readRequest();
}

QJsonArray
CommModbus::registersToJsonArray(const Registers &registers)
{
    QJsonArray regs;

    auto keys = registers.keys();

    for (const auto &key : keys)
    {
        QJsonObject reg;
        reg.insert("register", key);
        reg.insert("value", registers.value(key));
        regs.append(reg);
    }

    return regs;
}
