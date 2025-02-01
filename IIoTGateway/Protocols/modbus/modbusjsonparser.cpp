#include "modbusjsonparser.h"

#include <QJsonArray>
#include <QJsonObject>

ModbusJsonParser::ModbusJsonParser(const QByteArray &data, quint16 maxEntries)
{
    m_document = QJsonDocument::fromJson(data);
    m_maxEntries = maxEntries;
}

ModbusJsonParser::Request
ModbusJsonParser::readRequest() const
{
    Request request;

    QJsonObject jsonObj = m_document.object();
    QJsonArray devices = jsonObj.value("devices").toArray();

    for (const auto device : devices)
    {
        QJsonObject deviceObj = device.toObject();
        quint8 address = deviceObj.value("address").toInt(1);

        QModbusDataUnit::RegisterType type = getType(deviceObj.value("type").toString());
        quint16 startRegister = deviceObj.value("startRegister").toInt(0);
        quint16 numberOfEntries = deviceObj.value("numberOfEntries").toInt(1);

        for (int i = 0, r = startRegister; i < numberOfEntries; i += m_maxEntries, r += m_maxEntries)
        {
            quint16 entries = qMin(m_maxEntries, quint16(numberOfEntries - i));
            request[address].append(QModbusDataUnit(type, r, entries));
        }
    }

    if (!request.isEmpty())
    {
        auto sortRequest = [](const auto &a, const auto &b) -> bool
        {
            quint8 addressA = a->key();
            quint8 addressB = b->key();

            QModbusDataUnit unitA = a->value();
            QModbusDataUnit unitB = b->value();

            return ((addressA < addressB) && (unitA.startAddress() < unitB.startAddress()));
        };
    }

    return request;
}

ModbusJsonParser::Request
ModbusJsonParser::writeRequest() const
{
#warning // TODO: implement write request
    return Request();
}

ModbusJsonParser::Addresses
ModbusJsonParser::sortedAddress(const Request &request)
{
    auto keys = request.keys();

    std::sort(keys.begin(), keys.end());

    return keys;
}

void
ModbusJsonParser::sortRequestUnits(ModbusJsonParser::Units &units)
{
    auto sort = [](const QModbusDataUnit &a, const QModbusDataUnit &b) -> bool
    {
        return a.startAddress() < b.startAddress();
    };

    std::sort(units.begin(), units.end(), sort);
}

QModbusDataUnit::RegisterType
ModbusJsonParser::getType(const QString &type) const
{
    if (type.toLower() == "coils")
    {
        return QModbusDataUnit::Coils;
    }
    else if (type.toLower() == "discreteinputs")
    {
        return QModbusDataUnit::DiscreteInputs;
    }
    else if (type.toLower() == "holdingregisters")
    {
        return QModbusDataUnit::HoldingRegisters;
    }
    else if (type.toLower() == "inputregisters")
    {
        return QModbusDataUnit::InputRegisters;
    }

    return QModbusDataUnit::Invalid;
}
