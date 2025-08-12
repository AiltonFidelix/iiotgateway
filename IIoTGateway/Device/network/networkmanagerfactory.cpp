#include "networkmanagerfactory.h"

NETWORK_BEGIN_NAMESPACE

NetworkManagerFactory::NetworkManagerList *NetworkManagerFactory::m_managerList = nullptr;

NetworkManager *NetworkManagerFactory::getNetworkManager(const QString &platform)
{
    if (m_managerList->contains(platform))
    {
        return m_managerList->value(platform)();
    }

    return nullptr;
}

int NetworkManagerFactory::registerNetworkManager(const QString &platform, Creator creator)
{
    if (m_managerList == nullptr)
    {
        m_managerList = new NetworkManagerList();
    }

    m_managerList->insert(platform, creator);

    return m_managerList->size();
}
NETWORK_END_NAMESPACE



