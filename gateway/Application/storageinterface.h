#ifndef STORAGEINTERFACE_H
#define STORAGEINTERFACE_H

#include <QString>
#include <QStringView>
#include <QJsonObject>

class StorageInterface
{
public:
    virtual ~StorageInterface() = default;

    virtual bool verify() const = 0;

    virtual bool setActive(bool active) const = 0;
    virtual bool setCloudProtocol(const QString &protocol) const = 0;
    virtual bool setEdgeProtocol(const QString &protocol) const = 0;
    virtual bool setProtocolSettings(const QString &protocol, const QJsonObject &settings) const = 0;

    virtual bool active() const = 0;
    virtual QString cloudProtocol() const = 0;
    virtual QString edgeProtocol() const = 0;
    virtual QJsonObject protocolSettings(const QString &protocol) const = 0;
    virtual QPair<QString, QString> userCredentials() const = 0;
};

#endif // STORAGEINTERFACE_H
