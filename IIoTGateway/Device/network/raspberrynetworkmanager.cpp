#include "raspberrynetworkmanager.h"

#include <QDebug>
#include <QProcess>
#include <QRegularExpression>

// setIPConfig("Wired connection 1", false, "192.168.1.50", "192.168.1.1", "8.8.8.8");

NETWORK_BEGIN_NAMESPACE

QString RaspberryNetworkManager::runCommand(const QString &program, const QStringList &args)
{
    QProcess process;
    process.start(program, args);
    process.waitForFinished();

    const QString output = process.readAllStandardOutput();
    const QString error = process.readAllStandardError();

    return output + error;
}

QString RaspberryNetworkManager::nmcliCommand(const QStringList &args)
{
    return runCommand(QStringLiteral("nmcli"), args);
}

QString RaspberryNetworkManager::activeInterface()
{
    const QString output = runCommand(QStringLiteral("ip"), {QStringLiteral("route")});
    const QStringList lines = output.split('\n');

    for (const QString &line : lines)
    {
        if (line.startsWith(QStringLiteral("default")))
        {
            static const QRegularExpression re("dev\\s+(\\w+)");
            const QRegularExpressionMatch match = re.match(line);

            if (match.hasMatch())
            {
                return match.captured(1);  // "eth0" or "wlan0"
            }
        }
    }

    return QStringLiteral("unknown");
}

QString RaspberryNetworkManager::connectionDetails(const QString &connectionName)
{
    return nmcliCommand({
        QStringLiteral("connection"),
        QStringLiteral("show"),
        connectionName,
        QStringLiteral("--show-secrets")
    });
}

void RaspberryNetworkManager::switchToWiFi()
{
    nmcliCommand({
        QStringLiteral("device"),
        QStringLiteral("disconnect"),
        QStringLiteral("eth0")
    });
    nmcliCommand({
        QStringLiteral("connection"),
        QStringLiteral("up"),
        QStringLiteral("preconfigured")
    });
}

void RaspberryNetworkManager::switchToEthernet()
{
    nmcliCommand({
        QStringLiteral("device"),
        QStringLiteral("disconnect"),
        QStringLiteral("wlan0")
    });

    nmcliCommand({
        QStringLiteral("device"),
        QStringLiteral("connect"),
        QStringLiteral("eth0")
    });
}

void RaspberryNetworkManager::connectToWiFi(const QString &ssid, const QString &password)
{
    const QStringList args{
        QStringLiteral("device"),
        QStringLiteral("wifi"),
        QStringLiteral("connect"),
        ssid,
        QStringLiteral("password"),
        password,
        QStringLiteral("ifname"),
        QStringLiteral("wlan0")
    };

    const QString result = nmcliCommand(args);

    qDebug() << "Wi-Fi connection result:" << result;
}

void RaspberryNetworkManager::setIPConfig(const QString &connectionName,
                                          const bool useDHCP,
                                          const QString &ip,
                                          const QString &gateway,
                                          const QString &dns)
{
    if (useDHCP)
    {
        nmcliCommand({
            QStringLiteral("con"),
            QStringLiteral("mod"),
            connectionName,
            QStringLiteral("ipv4.method"),
            QStringLiteral("auto")
        });
    }
    else
    {
        if (ip.isEmpty() || gateway.isEmpty())
        {
            qWarning() << "Static IP config requires IP and gateway";
            return;
        }

        nmcliCommand({
            QStringLiteral("con"),
            QStringLiteral("mod"),
            connectionName,
            QStringLiteral("ipv4.addresses"),
            QString("%1/24").arg(ip),
            QStringLiteral("ipv4.gateway"),
            gateway,
            QStringLiteral("ipv4.method"),
            QStringLiteral("manual")
        });

        if (!dns.isEmpty())
        {
            nmcliCommand({
                QStringLiteral("con"),
                QStringLiteral("mod"),
                connectionName,
                QStringLiteral("ipv4.dns"),
                dns
            });
        }
    }

    nmcliCommand({
        QStringLiteral("con"),
        QStringLiteral("up"),
        connectionName
    });
}


NETWORK_END_NAMESPACE
