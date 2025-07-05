#ifndef STORAGEINTERFACE_H
#define STORAGEINTERFACE_H

#include <QString>
#include <QJsonObject>

class StorageInterface
{
public:
    virtual ~StorageInterface() = default;

    virtual bool verify() = 0;

    virtual bool setActive(bool active) = 0;
    virtual bool setCloudProtocol(const QString &protocol) = 0;
    virtual bool setEdgeProtocol(const QString &protocol) = 0;
    virtual bool setProtocolSettings(const QString &protocol, const QJsonObject &settings) = 0;

    virtual bool active() = 0;
    virtual QString cloudProtocol() = 0;
    virtual QString edgeProtocol() = 0;
    virtual QJsonObject protocolSettings(const QString &protocol) = 0;
    virtual QPair<QString, QString> userCredentials() = 0;
};

#endif // STORAGEINTERFACE_H
