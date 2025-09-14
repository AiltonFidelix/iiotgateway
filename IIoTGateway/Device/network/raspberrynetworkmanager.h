#ifndef RASPBERRYNETWORKMANAGER_H
#define RASPBERRYNETWORKMANAGER_H

#include "network_global.h"
#include "networkmanager.h"

#include <QString>

NETWORK_BEGIN_NAMESPACE

class RaspberryNetworkManager : public NetworkManager
{
public:
    static int m_id;

    QString runCommand(const QString &program, const QStringList &args);
    QString nmcliCommand(const QStringList &args);
    QString activeInterface();
    QString connectionDetails(const QString &connectionName);

    void switchToWiFi();
    void switchToEthernet();
    void connectToWiFi(const QString &ssid, const QString &password);

    void setIPConfig(const QString &connectionName,
                     const bool useDHCP = true,
                     const QString &ip = QString(),
                     const QString &gateway = QString(),
                     const QString &dns = QString());

public:
    bool load() override;
    bool save() const override;
    bool setSettings(const QByteArray &settings) override;
    QByteArray settings() const override;

    static NetworkManager *create();
};

NETWORK_END_NAMESPACE

#endif // RASPBERRYNETWORKMANAGER_H
