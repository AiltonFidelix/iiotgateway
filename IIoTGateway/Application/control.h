#ifndef CONTROL_H
#define CONTROL_H

#include <QObject>
#include <QHttpServer>

class Control : public QObject
{
    Q_OBJECT
public:
    explicit Control(QObject *parent = nullptr);
    ~Control();

    bool start(int port = 8083);

signals:

private:
    void registerRoutes();

private:
    QHttpServer m_httpServer;
};

#endif // CONTROL_H
