#ifndef CONTROL_H
#define CONTROL_H

#include <QObject>
#include <QHttpServer>

class StorageInterface;
class Gateway;

class Control : public QObject
{
    Q_OBJECT

    QHttpServer m_httpServer;
    Gateway *m_gateway;
    StorageInterface *m_storage;

    void registerRoutes();

public:
    explicit Control(QObject *parent = nullptr);
    ~Control();

    bool start(int port = 8084);
    void stop();

    void setGateway(Gateway *gateway);
    void setStorage(StorageInterface *storage);
};

#endif // CONTROL_H
