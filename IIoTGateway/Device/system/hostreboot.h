#ifndef HOSTREBOOT_H
#define HOSTREBOOT_H

#include "system_global.h"
#include "reboot.h"

SYSTEM_BEGIN_NAMESPACE

class HostReboot : public Reboot
{
    static int m_id;

public:
    void reboot() const override;

    static Reboot *create();
};

SYSTEM_END_NAMESPACE

#endif // HOSTREBOOT_H
