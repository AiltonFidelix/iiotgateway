#ifndef RASPBERRYNETWORKMANAGER_H
#define RASPBERRYNETWORKMANAGER_H

#include "network_global.h"
#include "networkmanager.h"

#include <QString>
#include <QMap>

NETWORK_BEGIN_NAMESPACE

class RaspberryNetworkManager : public NetworkManager
{
    using NetSettings = QMap<QString, QString>;

    static int m_id;

    QString m_interface;
    QString m_settings;

    QString runCommand(const QString &program, const QStringList &args, bool *ok = nullptr) const;
    QString nmcliCommand(const QStringList &args, bool *ok = nullptr) const;
    QString activeInterface() const;
    QString connectionDetails(const QString &connectionName) const;

    void switchToWiFi();
    void switchToEthernet();
    void connectToWiFi(const QString &ssid, const QString &password);

    void setIPConfig(const QString &connectionName,
                     const bool useDHCP = true,
                     const QString &ip = QString(),
                     const QString &gateway = QString(),
                     const QString &dns = QString());


    NetSettings settingsParser() const;

public:

    RaspberryNetworkManager();

    bool load() override;
    bool save() const override;
    bool setSettings(const QByteArray &settings) override;
    QByteArray settings() const override;

    static NetworkManager *create();
};

NETWORK_END_NAMESPACE

#endif // RASPBERRYNETWORKMANAGER_H
