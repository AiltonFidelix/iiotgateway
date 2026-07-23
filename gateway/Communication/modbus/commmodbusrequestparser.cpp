#include "commmodbusrequestparser.hpp"

#include <QJsonArray>
#include <QJsonObject>

COMM_MODBUS_BEGIN_NAMESPACE

CommModbusRequestParser::CommModbusRequestParser(QByteArray data, quint16 maxEntries)
    : _maxEntries{maxEntries},
      _request{},
      _type{RequestType::Unknown} {
    parser(QJsonDocument::fromJson(std::move(data)));
}

Request CommModbusRequestParser::request() const {
    return _request;
}

RequestType CommModbusRequestParser::type() const {
    return _type;
}

Addresses CommModbusRequestParser::sortedAddress(const Request &request) {
    auto keys = request.keys();

    std::ranges::sort(keys);

    return keys;
}

void CommModbusRequestParser::sortRequestUnits(Units &units) {
    auto sort = [](const QModbusDataUnit &a, const QModbusDataUnit &b) -> bool {
        return a.startAddress() < b.startAddress();
    };

    std::ranges::sort(units, sort);
}

void CommModbusRequestParser::parser(const QJsonDocument &document) {
    auto setValues = [](QJsonArray::Iterator &current, const QJsonArray &values, QModbusDataUnit &unit) -> void {
        for (qsizetype i = 0, total = unit.valueCount(); i < total; ++i) {
            if (current == values.end())
                break;

            unit.setValue(i, current->toInt());
            current++;
        }
    };

    const QJsonObject jsonObj = document.object();
    const QJsonArray devices = jsonObj.value(QStringLiteral("devices")).toArray();

    // TODO Move this process to workers threads
    // Get a thread pool and feed them with each device, when everything is finished put into _request

    for (const auto &device : devices) {
        const QJsonObject deviceObj = device.toObject();
        const auto address = static_cast<quint8>(deviceObj.value(QStringLiteral("address")).toInt(0));

        const auto registertype = getType(deviceObj.value(QStringLiteral("registerType")).toString());
        const auto startRegister = static_cast<quint16>(deviceObj.value(QStringLiteral("startRegister")).toInt(0));
        const auto numberOfEntries = static_cast<quint16>(deviceObj.value(QStringLiteral("numberOfEntries")).toInt(0));

        bool hasValues = deviceObj.contains(QStringLiteral("values"));

        QJsonArray values{};
        QJsonArray::Iterator current{};

        if (hasValues) {
            values = deviceObj.value(QStringLiteral("values")).toArray();
            current = values.begin();
            _type = RequestType::Write;
        } else {
            _type = RequestType::Read;
        }

        for (int i = 0, r = startRegister; i < numberOfEntries; i += _maxEntries, r += _maxEntries) {
            const quint16 entries = std::min(_maxEntries, quint16(numberOfEntries - i));

            QModbusDataUnit unit(registertype, r, entries);

            if (hasValues) {
                setValues(current, values, unit);
            }

            _request[address].append(unit);
        }
    }
}

QModbusDataUnit::RegisterType CommModbusRequestParser::getType(const QString &type) const {
    const QString typeLower = type.toLower();

    if (typeLower == "coils") {
        return QModbusDataUnit::Coils;
    } else if (typeLower == "discreteinputs") {
        return QModbusDataUnit::DiscreteInputs;
    } else if (typeLower == "holdingregisters") {
        return QModbusDataUnit::HoldingRegisters;
    } else if (typeLower == "inputregisters") {
        return QModbusDataUnit::InputRegisters;
    }

    return QModbusDataUnit::Invalid;
}

COMM_MODBUS_END_NAMESPACE
