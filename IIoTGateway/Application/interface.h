#ifndef INTERFACE_H
#define INTERFACE_H

#include <QObject>

// #include <QCoreApplication>
// #include <QtHttpServer/QHttpServer>
// #include <QtConcurrent/QtConcurrent>
// #include <QRandomGenerator>
// #include <QJsonObject>

class ControlServer : public QObject
{
    Q_OBJECT
public:
    explicit ControlServer(QObject *parent = nullptr);

signals:
};

#endif // INTERFACE_H
