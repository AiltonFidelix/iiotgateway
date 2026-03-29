#ifndef REBOOTFACTORY_H
#define REBOOTFACTORY_H

#include <functional>

#include "system_global.h"
#include "reboot.h"

SYSTEM_BEGIN_NAMESPACE

class RebootFactory
{
    using Creator = std::function<Reboot*()>;

    static Creator m_creator;

public:
    RebootFactory() = delete;

    static Reboot *create();
    static bool registerCreator(Creator creator);
};

SYSTEM_END_NAMESPACE

#endif // REBOOTFACTORY_H
