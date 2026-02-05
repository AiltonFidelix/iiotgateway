#include "rebootfactory.h"

SYSTEM_BEGIN_NAMESPACE

RebootFactory::RebootList *RebootFactory::m_rebootList = nullptr;

Reboot *RebootFactory::create(const QString &platform)
{
    if (m_rebootList->contains(platform))
    {
        return m_rebootList->value(platform)();
    }

    return nullptr;
}

int RebootFactory::registerCreator(const QString &platform, Creator creator)
{
    if (m_rebootList == nullptr)
    {
        m_rebootList = new RebootList();
    }

    m_rebootList->insert(platform, creator);

    return m_rebootList->size();
}

SYSTEM_END_NAMESPACE






