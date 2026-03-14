#include "linuxreboot.h"
#include "rebootfactory.h"

#include <QDebug>
#include <QProcess>

SYSTEM_BEGIN_NAMESPACE

bool LinuxReboot::m_registered = RebootFactory::registerCreator(LinuxReboot::create);

void LinuxReboot::reboot() const
{
    qDebug() << "Rebooting...";

    QProcess process;
    process.start(QStringLiteral("reboot"), QStringList());

    if (!process.waitForStarted())
    {
        qWarning() << "Failed to start reboot process";
        return;
    }

    process.waitForFinished();
    
    qDebug() << "Reboot command sent";
}

Reboot *LinuxReboot::create()
{
    return static_cast<Reboot*>(new LinuxReboot());
}

SYSTEM_END_NAMESPACE
