#ifndef INTERFACE_H
#define INTERFACE_H

#include <QObject>
#include <QHttpServer>

class ControlServer : public QObject
{
    Q_OBJECT
public:
    explicit ControlServer(QObject *parent = nullptr);
    ~ControlServer();

    bool start(int port = 8083);

signals:

private:
    void registerRoutes();

private:
    QHttpServer m_httpServer;
};

#endif // INTERFACE_H
