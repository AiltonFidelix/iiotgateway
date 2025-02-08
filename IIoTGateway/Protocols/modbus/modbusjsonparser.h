#ifndef MODBUSJSONPARSER_H
#define MODBUSJSONPARSER_H

#include <QObject>
#include <QJsonDocument>
#include <QModbusDataUnit>

class ModbusJsonParser : public QObject
{
    Q_OBJECT
public:

    using Addresses = QList<quint8>;
    using Units = QList<QModbusDataUnit>;
    using Request = QHash<quint8, Units>;

    enum RequestType {
        Unknown = 0,
        Read,
        Write
    };

    ModbusJsonParser(const QByteArray &data, quint16 maxEntries = 10);

    Request request();

    /*
     * @brief The type is defined after request() be called
     * @see request()
     */
    RequestType type();

    static Addresses sortedAddress(const Request &request);
    static void sortRequestUnits(Units &units);

private:
    QModbusDataUnit::RegisterType getType(const QString &type) const;

private:
    QJsonDocument m_document;
    quint16 m_maxEntries;
    RequestType m_type;
};

#endif // MODBUSJSONPARSER_H
