#ifndef CONTROL_H
#define CONTROL_H

#include <QObject>
#include <QHttpServer>

class StorageInterface;
class Gateway;

class Control : public QObject
{
    Q_OBJECT
public:
    explicit Control(QObject *parent = nullptr);
    ~Control();

    bool start(int port = 8084);
    void stop();

    void setGateway(Gateway *gateway);
    void setStorage(StorageInterface *storage);

private:
    void registerRoutes();

private:
    QHttpServer m_httpServer;
    Gateway *m_gateway;
    StorageInterface *m_storage;
};

#endif // CONTROL_H
