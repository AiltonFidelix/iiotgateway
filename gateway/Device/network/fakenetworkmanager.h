#ifndef FAKENETWORKMANAGER_H
#define FAKENETWORKMANAGER_H

#include "network_global.h"
#include "networkmanager.h"

NETWORK_BEGIN_NAMESPACE

class FakeNetworkManager : public NetworkManager
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

#endif // FAKENETWORKMANAGER_H
