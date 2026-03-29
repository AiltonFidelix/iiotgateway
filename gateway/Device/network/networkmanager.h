#ifndef NETWORKMANAGER_H
#define NETWORKMANAGER_H

#include <QByteArray>

#include "network_global.h"

NETWORK_BEGIN_NAMESPACE

class NetworkManager
{
public:
    virtual ~NetworkManager() = default;

    virtual bool load() = 0;
    virtual bool save() const = 0;
    virtual bool setSettings(const QByteArray &settings) = 0;
    virtual QByteArray settings() const = 0;
};

NETWORK_END_NAMESPACE

#endif // NETWORKMANAGER_H
