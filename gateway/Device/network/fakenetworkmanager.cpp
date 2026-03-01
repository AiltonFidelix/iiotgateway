#include "fakenetworkmanager.h"
#include "networkmanagerfactory.h"

#include <QDebug>

NETWORK_BEGIN_NAMESPACE

int FakeNetworkManager::m_id = NetworkManagerFactory::registerNetworkManager(QStringLiteral("host"), FakeNetworkManager::create);

bool FakeNetworkManager::load()
{
    return true;
}

bool FakeNetworkManager::save() const
{
    return true;
}

bool FakeNetworkManager::setSettings(const QByteArray &settings)
{
    if (!settings.isEmpty())
    {
        qDebug() << "Fake network settings received:" << settings;

        return true;
    }

    return false;
}

QByteArray FakeNetworkManager::settings() const
{
    const QByteArray settings{
        R"(
            {
                "network": "wlan0",
                "method": "manual",
                "ipv4": "192.168.4.45",
                "netmask": "255.255.255.0",
                "router": "192.168.4.1",
                "dns": "8.8.8.8",
                "wifi": {
                    "ssid": "FakeNet",
                    "password": "FakePass"
                }
            }
        )"
    };

    qDebug() << "Returning fake network settings:" << settings;

    return settings;
}

NetworkManager *FakeNetworkManager::create()
{
    return static_cast<NetworkManager*>(new FakeNetworkManager());
}

NETWORK_END_NAMESPACE
