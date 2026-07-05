#ifndef COMMMODBUSREQUESTPARSER_H
#define COMMMODBUSREQUESTPARSER_H

#include "commmodbus_global.hpp"

#include <QJsonDocument>
#include <QModbusDataUnit>

COMM_MODBUS_BEGIN_NAMESPACE

using Addresses = QList<quint8>;
using Units = QList<QModbusDataUnit>;
using Request = QHash<quint8, Units>;

enum class RequestType : quint8 {
    Unknown = 0,
    Read,
    Write
};

class CommModbusRequestParser
{
public:
    explicit CommModbusRequestParser(QByteArray data, quint16 maxEntries = 10);

    Request request() const;
    RequestType type() const;

    static Addresses sortedAddress(const Request &request);
    static void sortRequestUnits(Units &units);

private:
    void parser(const QJsonDocument &document);
    QModbusDataUnit::RegisterType getType(const QString &type) const;

    quint16 m_maxEntries;
    Request m_request;
    RequestType m_type;
};

COMM_MODBUS_END_NAMESPACE

#endif // COMMMODBUSREQUESTPARSER_H
