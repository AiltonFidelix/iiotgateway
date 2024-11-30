#include "modbusjsonparser.h"

ModbusJsonParser::ModbusJsonParser(const QByteArray &data)
{
    m_document = QJsonDocument::fromJson(data);
}

ModbusJsonParser::Request
ModbusJsonParser::readRequest() const
{
    return Request();
}

ModbusJsonParser::Request
ModbusJsonParser::writeRequest() const
{
    return Request();
}
