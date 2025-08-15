#ifndef CONTROL_H
#define CONTROL_H

#include <QObject>
#include <QTimer>
#include <QHttpServer>

#include "driver/gpio/gpiopin.h"
#include "network/networkmanager.h"

class StorageInterface;
class Gateway;

using device::driver::gpio::GPIOPin;
using device::network::NetworkManager;

class Control : public QObject
{
    Q_OBJECT

    QHttpServer m_httpServer;
    QTimer m_ledTimer;
    Gateway *m_gateway;
    StorageInterface *m_storage;
    GPIOPin *m_ledPin;
    NetworkManager *m_networkManager;

    void registerRoutes();
    QHttpServerResponse makeResponse(const QByteArray &data) const;
    QHttpServerResponse makeResponse(bool ok, const QString &message) const;

public:
    explicit Control(const QString &platform, QObject *parent = nullptr);
    ~Control();

    bool start(int port = 8084);
    void stop();

    void setGateway(Gateway *gateway);
    void setStorage(StorageInterface *storage);
};

#endif // CONTROL_H
