#ifndef HOSTNETWORKMANAGER_H
#define HOSTNETWORKMANAGER_H

#include "network_global.h"
#include "networkmanager.h"

NETWORK_BEGIN_NAMESPACE

class HostNetworkManager : public NetworkManager
{
    static int m_id;

public:
    bool load() override;
    bool save() const override;
    bool setSettings(const QByteArray &settings) override;
    QByteArray settings() const override;

    static NetworkManager *create();
};

NETWORK_END_NAMESPACE

#endif // HOSTNETWORKMANAGER_H
