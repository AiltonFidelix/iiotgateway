#include "hostreboot.h"
#include "rebootfactory.h"

#include <QDebug>

SYSTEM_BEGIN_NAMESPACE

int HostReboot::m_id = RebootFactory::registerCreator(QStringLiteral("host"), HostReboot::create);

void HostReboot::reboot() const
{
    // TODO: Kill the process and restart
    qDebug() << "Host rebooting...";
}

Reboot *HostReboot::create()
{
    return static_cast<Reboot*>(new HostReboot());
}

SYSTEM_END_NAMESPACE
