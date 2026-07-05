#include "commmodbus.hpp"

#include <QCoreApplication>
#include <QEventLoop>
#include <QFile>
#include <QJsonArray>
#include <QJsonObject>

COMM_MODBUS_BEGIN_NAMESPACE

CommModbus::CommModbus(QJsonObject settings)
    : m_modbusClient{nullptr},
    m_polling{nullptr},
    m_readRequest{},
    m_settingsParser{std::move(settings)}
{
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

void CommModbus::disconnectComm()
{
    if (!isconnected())
    {
        m_modbusClient->disconnectDevice();
    }

    emit disconnected();
}

bool CommModbus::isconnected() const
{
    return (m_modbusClient->state() == QModbusDevice::ConnectedState);
}

CommModbusClientInterface *CommModbus::modbusClient()
{
    return m_modbusClient;
}

void CommModbus::setModbusClient(CommModbusClientInterface *client)
{
    m_modbusClient = client;
}

void CommModbus::incoming(QByteArray data)
{
    CommModbusRequestParser parser(std::move(data));

    const Request request = parser.request();
    const RequestType type = parser.type();

    switch (type)
    {
        case RequestType::Write:
            qInfo() << "Received a write request";
            writeRegisters(request);
            break;
        case RequestType::Read:
            qInfo() << "Received a read request";
            readRegisters(request);
        default:
            qWarning() << "Received an unknown request";
            break;
    }
}

void CommModbus::stateChanged(QModbusDevice::State state)
{
    switch (state)
    {
        case QModbusDevice::UnconnectedState:
            qInfo() << "Modbus received unconnected state";
            emit disconnected();
            break;
        case QModbusDevice::ConnectedState:
            qInfo() << "Modbus received connected state";
            emit connected();
            break;
        case QModbusDevice::ConnectingState:
            qInfo() << "Modbus received connecting state";
            emit connected();
            break;
        case QModbusDevice::ClosingState:
            qInfo() << "Modbus received closing state";
            emit connected();
            break;
        default:
            break;
    }
}

void CommModbus::readRegisters(const Request &request)
{
    QJsonArray devices{};

    const Addresses addresses = CommModbusRequestParser::sortedAddress(request);

    for (const auto &address : addresses)
    {
        QJsonObject device{};
        QJsonArray errors{};

        Registers registers{};
        Units units = request.value(address);
        CommModbusRequestParser::sortRequestUnits(units);

        for (const auto &unit : std::as_const(units))
        {
            if (QModbusReply *reply = m_modbusClient->sendReadRequest(unit, address))
            {
                QEventLoop loop;
                connect(reply, &QModbusReply::finished, &loop, &QEventLoop::quit);

                loop.exec();
                registers.insert(readReady(reply));
            }
            else
            {
                const auto errorMessage = QString("Read error: %1").arg(m_modbusClient->errorString());
                errors.append(errorMessage);
                emit error(errorMessage.toUtf8());
            }
        }

        device.insert(QStringLiteral("address"), address);
        device.insert(QStringLiteral("registers"), registersToJsonArray(registers));

        if (!errors.isEmpty())
        {
            device.insert(QStringLiteral("errors"), errors);
        }

        devices.append(device);
    }

    if (!devices.isEmpty())
    {
        QJsonObject json{};

        json.insert(QStringLiteral("datetime"), QDateTime::currentDateTime().toString(QStringLiteral("dd/MM/yyyy hh:mm:ss.zzz")));
        json.insert(QStringLiteral("devices"), devices);

        QByteArray data{QJsonDocument(json).toJson(QJsonDocument::Compact)};

        emit outgoing(std::move(data));
    }
}

void CommModbus::writeRegisters(const Request &request)
{
    const Addresses addresses = CommModbusRequestParser::sortedAddress(request);

    for (const auto &address : addresses)
    {
        const Units units = request.value(address);

        for (const auto &unit : units)
        {
            if (QModbusReply *reply = m_modbusClient->sendWriteRequest(unit, address))
            {
                QEventLoop loop;
                connect(reply, &QModbusReply::finished, &loop, &QEventLoop::quit);
                loop.exec();

                reply->deleteLater();
            }
            else
            {
                emit error(QString("Write error: %1").arg(m_modbusClient->errorString()).toUtf8());
            }
        }
    }
}

Registers CommModbus::readReady(QModbusReply *reply)
{
    Registers registers{};

    if (!reply)
    {
        return registers;
    }

    if (reply->error() == QModbusDevice::NoError)
    {
        const auto unit = reply->result();
        const qsizetype valueCount = unit.valueCount();

        for (qsizetype i = 0, total = valueCount; i < total; ++i)
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

bool CommModbus::ispolling()
{
    return (m_polling != nullptr);
}

void CommModbus::initPolling()
{
    if (ispolling())
    {
        m_readRequest = m_settingsParser.requests();

        connect(m_polling, &QTimer::timeout, this, &CommModbus::pollingCallback);

        m_polling->start();
    }
}

void CommModbus::pollingCallback()
{
    m_polling->stop();

    readRegisters(m_readRequest);

    m_polling->start();
}

QJsonArray CommModbus::registersToJsonArray(const Registers &registers)
{
    QJsonArray regs{};

    const QList<quint16> keys = registers.keys();

    for (const auto &key : keys)
    {
        QJsonObject reg{};
        reg.insert(QStringLiteral("register"), key);
        reg.insert(QStringLiteral("value"), registers.value(key));
        regs.append(reg);
    }

    return regs;
}

COMM_MODBUS_END_NAMESPACE
