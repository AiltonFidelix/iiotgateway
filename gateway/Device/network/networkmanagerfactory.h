#ifndef NETWORKMANAGERFACTORY_H
#define NETWORKMANAGERFACTORY_H

#include <functional>
#include <QByteArray>
#include <QHash>

#include "network_global.h"
#include "networkmanager.h"

NETWORK_BEGIN_NAMESPACE

class NetworkManagerFactory
{
    using Creator = std::function<NetworkManager*()>;
    using NetworkManagerList = QHash<QString, Creator>;

    static NetworkManagerList *m_managerList;

public:
    NetworkManagerFactory() = delete;

    static NetworkManager *getNetworkManager(const QString &platform);
    static int registerNetworkManager(const QString &platform, Creator creator);
};

NETWORK_END_NAMESPACE

#endif // NETWORKMANAGERFACTORY_H
