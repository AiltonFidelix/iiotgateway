#include "fakereboot.h"
#include "rebootfactory.h"

#include <QDebug>
#include <QCoreApplication>

SYSTEM_BEGIN_NAMESPACE

bool FakeReboot::m_registered = RebootFactory::registerCreator(FakeReboot::create);

void FakeReboot::reboot() const
{
    qDebug() << "I will fakely reboot...";

    QCoreApplication::exit(0);
}

Reboot *FakeReboot::create()
{
    return static_cast<Reboot*>(new FakeReboot());
}

SYSTEM_END_NAMESPACE
