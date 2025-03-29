#ifndef DEVICECLIENT_H
#define DEVICECLIENT_H

#include <QObject>
#include <QNetworkAccessManager>

class DeviceClient : public QObject
{
    Q_OBJECT
public:
    explicit DeviceClient(QObject *parent = nullptr);

signals:
    void deviceSettings(QByteArray settings);
    void success(QByteArray message);
    void error(QByteArray error);

public slots:
    void setDeviceSettings(QByteArray settings);

private:
    QNetworkAccessManager m_manager;
    QString m_serverUrl;
};

#endif // DEVICECLIENT_H
