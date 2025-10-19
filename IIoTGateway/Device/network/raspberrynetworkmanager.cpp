#include "raspberrynetworkmanager.h"
#include "networkmanagerfactory.h"

#include <QDebug>
#include <QProcess>
#include <QRegularExpression>
#include <QJsonDocument>
#include <QJsonObject>

// setIPConfig("Wired connection 1", false, "192.168.1.50", "192.168.1.1", "8.8.8.8");

NETWORK_BEGIN_NAMESPACE

int RaspberryNetworkManager::m_id = NetworkManagerFactory::registerNetworkManager(QStringLiteral("raspberry"), RaspberryNetworkManager::create);

RaspberryNetworkManager::RaspberryNetworkManager()
    : m_interface(QStringLiteral("wlan0")),
    m_settings(QStringLiteral(""))
{
}

QString RaspberryNetworkManager::runCommand(const QString &program, const QStringList &args, bool *ok) const
{
    QProcess process;
    process.start(program, args);
    process.waitForFinished();

    const QString output = process.readAllStandardOutput();

    if (ok)
    {
        *ok = !process.errorString().isEmpty();
    }

    return output;
}

QString RaspberryNetworkManager::nmcliCommand(const QStringList &args, bool *ok) const
{
    return runCommand(QStringLiteral("nmcli"), args, ok);
}

QString RaspberryNetworkManager::activeInterface() const
{
    bool ok = false;

    const QString output = runCommand(QStringLiteral("ip"), {QStringLiteral("route")}, &ok);

    if (!ok)
    {
        return QStringLiteral("");
    }

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

QString RaspberryNetworkManager::connectionDetails(const QString &connectionName) const
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

RaspberryNetworkManager::NetSettings RaspberryNetworkManager::settingsParser() const
{
    const QStringList lines = m_settings.split('\n');

    NetSettings netSettings{};

    for (const QString &line : lines)
    {
        const QStringList content = line.split(':');

        if (content.count() != 2)
            continue;

        const QString property = content.at(0).trimmed();
        const QString value = content.at(1).trimmed();

        netSettings.insert(property, value);
    }

    return netSettings;
}

bool RaspberryNetworkManager::load()
{
    bool ok = false;

    m_interface = activeInterface();

    if (m_interface.isEmpty() || m_interface == QStringLiteral("unknown"))
    {
        return false;
    }

    const QString profile = m_interface == QStringLiteral("wlan0") ? QStringLiteral("preconfigured") : QStringLiteral("Wired connection 1");

    m_settings = connectionDetails(profile);

    return !m_settings.isEmpty();
}

bool RaspberryNetworkManager::save() const
{
    return true;
}

bool RaspberryNetworkManager::setSettings(const QByteArray &settings)
{
    return false;
}

QByteArray RaspberryNetworkManager::settings() const
{
    const NetSettings netSettings = settingsParser();

    QJsonObject netObject{};

    netObject.insert(QStringLiteral("network"), m_interface);
    netObject.insert(QStringLiteral("method"), netSettings.value(QStringLiteral("ipv4.method")));
    netObject.insert(QStringLiteral("ipv4"), netSettings.value(QStringLiteral("IP4.ADDRESS[1]")).remove("/24"));
    netObject.insert(QStringLiteral("router"), netSettings.value(QStringLiteral("IP4.GATEWAY")));
    netObject.insert(QStringLiteral("dns"), QStringLiteral("")); // TODO: parse

    if (m_interface == QStringLiteral("wlan0"))
    {
        QJsonObject wifiObject{};

        wifiObject.insert(QStringLiteral("ssid"), netSettings.value(QStringLiteral("802-11-wireless.ssid")));
        wifiObject.insert(QStringLiteral("password"), QStringLiteral("")); // TODO: parse

        netObject.insert(QStringLiteral("wifi"), wifiObject);
    }

    return QJsonDocument(netObject).toJson(QJsonDocument::Compact);
}

NetworkManager *RaspberryNetworkManager::create()
{
    return static_cast<NetworkManager*>(new RaspberryNetworkManager());
}

NETWORK_END_NAMESPACE
