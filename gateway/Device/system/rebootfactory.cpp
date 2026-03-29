#include "rebootfactory.h"

SYSTEM_BEGIN_NAMESPACE

RebootFactory::Creator RebootFactory::m_creator = nullptr;

Reboot *RebootFactory::create()
{
    if (m_creator)
    {
        return m_creator();
    }

    return nullptr;
}

bool RebootFactory::registerCreator(Creator creator)
{
    m_creator = creator;

    return true;
}

SYSTEM_END_NAMESPACE






