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

    ModbusJsonParser(const QByteArray &data);

    Request readRequest() const;
    Request writeRequest() const;

private:
    QJsonDocument m_document;
};

#endif // MODBUSJSONPARSER_H
