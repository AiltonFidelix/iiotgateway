#ifndef STORAGE_H
#define STORAGE_H

#include <QString>
#include <QJsonDocument>

class Storage
{
public:
    virtual ~Storage() = default;

    virtual bool verify() = 0;

    virtual bool setActive(bool active) = 0;
    virtual bool setCloudProtocol(const QString &protocol) = 0;
    virtual bool setEdgeProtocol(const QString &protocol) = 0;
    virtual bool setProtocolSettings(const QString &protocol, const QJsonDocument &settings) = 0;

    virtual bool active() = 0;
    virtual QString cloudProtocol() = 0;
    virtual QString edgeProtocol() = 0;
    virtual QJsonDocument protocolSettings(const QString &protocol) = 0;
};

#endif // STORAGE_H
