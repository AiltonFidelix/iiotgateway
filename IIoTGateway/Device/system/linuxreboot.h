#ifndef LINUXREBOOT_H
#define LINUXREBOOT_H

#include "system_global.h"
#include "reboot.h"

SYSTEM_BEGIN_NAMESPACE

class LinuxReboot : public Reboot
{
    static bool m_registered;

public:
    void reboot() const override;

    static Reboot *create();
};

SYSTEM_END_NAMESPACE

#endif // LINUXREBOOT_H
