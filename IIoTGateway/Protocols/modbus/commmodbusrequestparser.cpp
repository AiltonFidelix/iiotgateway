#include "commmodbusrequestparser.h"

#include <QJsonArray>
#include <QJsonObject>

COMM_MODBUS_BEGIN_NAMESPACE

CommModbusRequestParser::CommModbusRequestParser(const QByteArray &data, quint16 maxEntries) :
    m_document(QJsonDocument::fromJson(data)),
    m_maxEntries(maxEntries),
    m_type(RequestType::Unknown)
{
}

Request CommModbusRequestParser::request()
{
    auto setValues = [](auto &current, const QJsonArray &values, QModbusDataUnit &unit) -> void
    {
        for (int j = 0, total = int(unit.valueCount()); j < total; ++j)
        {
            if (current == values.end())
                break;

            unit.setValue(j, current->toInt());
            current++;
        }
    };

    Request request{};

    const QJsonObject jsonObj = m_document.object();
    const QJsonArray devices = jsonObj.value("devices").toArray();

    for (const auto &device : devices)
    {
        const QJsonObject deviceObj = device.toObject();
        const quint8 address = static_cast<quint8>(deviceObj.value("address").toInt(0));

        const auto registertype = getType(deviceObj.value("type").toString());
        const quint16 startRegister = deviceObj.value("startRegister").toInt(0);
        const quint16 numberOfEntries = deviceObj.value("numberOfEntries").toInt(0);

        bool hasValues = deviceObj.contains("values");

        QJsonArray values{};
        QJsonArray::Iterator current{};

        if (hasValues)
        {
            values = deviceObj.value("values").toArray();
            current = values.begin();
            m_type = RequestType::Write;
        }
        else
        {
            m_type = RequestType::Read;
        }

        for (int i = 0, r = startRegister; i < numberOfEntries; i += m_maxEntries, r += m_maxEntries)
        {
            const quint16 entries = qMin(m_maxEntries, quint16(numberOfEntries - i));

            auto unit = QModbusDataUnit(registertype, r, entries);

            if (hasValues)
            {
                setValues(current, values, unit);
            }

            request[address].append(unit);
        }
    }

    return request;
}

RequestType CommModbusRequestParser::type()
{
    return m_type;
}

Addresses CommModbusRequestParser::sortedAddress(const Request &request)
{
    auto keys = request.keys();

    std::sort(keys.begin(), keys.end());

    return keys;
}

void CommModbusRequestParser::sortRequestUnits(Units &units)
{
    auto sort = [](const QModbusDataUnit &a, const QModbusDataUnit &b) -> bool
    {
        return a.startAddress() < b.startAddress();
    };

    std::sort(units.begin(), units.end(), sort);
}

QModbusDataUnit::RegisterType CommModbusRequestParser::getType(const QString &type) const
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

COMM_MODBUS_END_NAMESPACE
