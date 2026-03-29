#ifndef FAKEREBOOT_H
#define FAKEREBOOT_H

#include "system_global.h"
#include "reboot.h"

SYSTEM_BEGIN_NAMESPACE

class FakeReboot : public Reboot
{
    static bool m_registered;

public:
    void reboot() const override;

    static Reboot *create();
};

SYSTEM_END_NAMESPACE

#endif // FAKEREBOOT_H
