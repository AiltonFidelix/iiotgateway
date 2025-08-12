#include "hostnetworkmanager.h"
#include "networkmanagerfactory.h"

#include <QDebug>

NETWORK_BEGIN_NAMESPACE

int HostNetworkManager::m_id = NetworkManagerFactory::registerNetworkManager("host", HostNetworkManager::create);

bool HostNetworkManager::load()
{
    return true;
}

bool HostNetworkManager::save() const
{
    return true;
}

bool HostNetworkManager::setSettings(const QByteArray &settings)
{
    if (!settings.isEmpty())
    {
        qDebug() << "Fake network settings received:" << settings;

        return true;
    }

    return false;
}

QByteArray HostNetworkManager::settings() const
{
    const QByteArray settings{
        R"(
            "network": "wlan0",
            "method": "manual",
            "ipv4": "192.168.4.45",
            "netmask": "255.255.255.0",
            "router": "192.168.4.1",
            "dns": "8.8.8.8",
            "wifi": {
                "ssdi": "FakeNet",
                "password": "FakePass"
            }
        )"
    };

    qDebug() << "Returning fake network settings:" << settings;

    return settings;
}

NetworkManager *HostNetworkManager::create()
{
    return static_cast<NetworkManager*>(new HostNetworkManager());
}

NETWORK_END_NAMESPACE
