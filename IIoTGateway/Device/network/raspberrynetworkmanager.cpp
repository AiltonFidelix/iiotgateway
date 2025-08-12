#include "raspberrynetworkmanager.h"

RaspberryNetworkManager::RaspberryNetworkManager() {}

// void createOrUpdateEthernet(const QString &ip, const QString &gateway, const QStringList &dns) {
//     QString conName = "my-ethernet";
//     QString dnsStr = dns.join(",");

//     // Delete if existing (optional for overwrite behavior)
//     QProcess::execute("nmcli", {"con", "delete", conName});

//     // Add new connection
//     QProcess::execute("nmcli", {
//                                    "con", "add", "type", "ethernet",
//                                    "ifname", "eth0",
//                                    "con-name", conName,
//                                    "ipv4.addresses", ip,
//                                    "ipv4.gateway", gateway,
//                                    "ipv4.dns", dnsStr,
//                                    "ipv4.method", "manual"
//                                });

//     qDebug() << "Ethernet config added/updated";
// }
// void createOrUpdateWiFi(const QString &ssid, const QString &password) {
//     QString conName = "my-wifi";

//     // Delete if existing (optional)
//     QProcess::execute("nmcli", {"con", "delete", conName});

//     // Add new Wi-Fi connection
//     QProcess::execute("nmcli", {
//                                    "con", "add", "type", "wifi",
//                                    "ifname", "wlan0",
//                                    "con-name", conName,
//                                    "ssid", ssid
//                                });

//     QProcess::execute("nmcli", {"con", "modify", conName, "wifi-sec.key-mgmt", "wpa-psk"});
//     QProcess::execute("nmcli", {"con", "modify", conName, "wifi-sec.psk", password});
//     QProcess::execute("nmcli", {"con", "modify", conName, "ipv4.method", "auto"});

//     qDebug() << "Wi-Fi config added/updated";
// }
// void createOrUpdateEthernet(const QString &ip, const QString &netmask,
//                             const QString &gateway, const QStringList &dns) {
//     QString conName = "my-ethernet";
//     QString dnsStr = dns.join(",");
//     QString ipWithCIDR = getIPWithCIDR(ip, netmask);

//     // Optional: delete previous config
//     QProcess::execute("nmcli", {"con", "delete", conName});

//     // Add new connection
//     QProcess::execute("nmcli", {
//                                    "con", "add", "type", "ethernet",
//                                    "ifname", "eth0",
//                                    "con-name", conName,
//                                    "ipv4.addresses", ipWithCIDR,
//                                    "ipv4.gateway", gateway,
//                                    "ipv4.dns", dnsStr,
//                                    "ipv4.method", "manual"
//                                });

//     QProcess::execute("nmcli", {"con", "modify", conName, "connection.autoconnect", "yes"});

//     qDebug() << "Ethernet configuration set:" << ipWithCIDR;
// }

// int cidrToNetmaskBits(const QString &cidr) {
//     bool ok;
//     int bits = cidr.toInt(&ok);
//     return ok ? bits : 24;
// }

// QString cidrToNetmask(int bits) {
//     quint32 mask = bits == 0 ? 0 : (~0U << (32 - bits));
//     QHostAddress addr(mask);
//     return addr.toString();
// }

// struct NetworkConfig {
//     QString ip;
//     QString netmask;
//     QString gateway;
//     QStringList dns;
// };

// NetworkConfig getConnectionConfig(const QString &connectionName) {
//     NetworkConfig config;

//     QProcess process;
//     process.start("nmcli", {"con", "show", connectionName});
//     process.waitForFinished();

//     QString output = process.readAllStandardOutput();
//     QStringList lines = output.split('\n', Qt::SkipEmptyParts);

//     for (const QString &line : lines) {
//         if (line.contains("ipv4.addresses:")) {
//             QString value = line.section(':', 1).trimmed(); // e.g. 192.168.1.150/24
//             QString ip = value.section('/', 0, 0);
//             QString cidr = value.section('/', 1, 1);
//             config.ip = ip;
//             config.netmask = cidrToNetmask(cidrToNetmaskBits(cidr));
//         }
//         if (line.contains("ipv4.gateway:")) {
//             config.gateway = line.section(':', 1).trimmed();
//         }
//         if (line.contains("ipv4.dns:")) {
//             QString dnsString = line.section(':', 1).trimmed();
//             config.dns = dnsString.split(',', Qt::SkipEmptyParts);
//         }
//     }

//     return config;
// }
