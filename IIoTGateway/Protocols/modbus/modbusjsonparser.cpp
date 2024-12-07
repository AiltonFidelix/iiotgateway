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
            request.insert(address, QModbusDataUnit(type, r, entries));
        }
    }

    return request;
}

ModbusJsonParser::Request
ModbusJsonParser::writeRequest() const
{
    return Request();
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
