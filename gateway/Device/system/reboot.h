#ifndef REBOOT_H
#define REBOOT_H

#include "system_global.h"

SYSTEM_BEGIN_NAMESPACE

class Reboot
{
public:
    virtual ~Reboot() = default;

    virtual void reboot() const = 0;
};

SYSTEM_END_NAMESPACE

#endif // REBOOT_H
