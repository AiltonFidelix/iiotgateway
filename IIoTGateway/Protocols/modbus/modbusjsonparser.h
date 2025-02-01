#ifndef MODBUSJSONPARSER_H
#define MODBUSJSONPARSER_H

#include <QObject>
#include <QJsonDocument>
#include <QModbusDataUnit>

class ModbusJsonParser : public QObject
{
    Q_OBJECT
public:
    // using Request = QMultiHash<quint8, QModbusDataUnit>;
    // using RequestIterator = QMultiHashIterator<quint8, QModbusDataUnit>;

    using Addresses = QList<quint8>;
    using Units = QList<QModbusDataUnit>;
    using Request = QHash<quint8, Units>;
    // using RequestIterator = QHashIterator<quint8, QList<QModbusDataUnit>>;

    ModbusJsonParser(const QByteArray &data, quint16 maxEntries = 10);

    Request readRequest() const;
    Request writeRequest() const;

    static Addresses sortedAddress(const Request &request);
    static void sortRequestUnits(Units &units);

private:
    QModbusDataUnit::RegisterType getType(const QString &type) const;

private:
    QJsonDocument m_document;
    quint16 m_maxEntries;
};

#endif // MODBUSJSONPARSER_H
