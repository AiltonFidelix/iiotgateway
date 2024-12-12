#ifndef MODBUSJSONPARSER_H
#define MODBUSJSONPARSER_H

#include <QObject>
#include <QJsonDocument>
#include <QModbusDataUnit>

class ModbusJsonParser : public QObject
{
    Q_OBJECT
public:
    using Request = QMultiHash<quint8, QModbusDataUnit>;
    using RequestIterator = QMultiHashIterator<quint8, QModbusDataUnit>;

    ModbusJsonParser(const QByteArray &data, quint16 maxEntries = 10);

    Request readRequest() const;
    Request writeRequest() const;

private:
    QModbusDataUnit::RegisterType getType(const QString &type) const;

private:
    QJsonDocument m_document;
    quint16 m_maxEntries;
};

#endif // MODBUSJSONPARSER_H
