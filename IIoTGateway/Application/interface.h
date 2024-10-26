#ifndef INTERFACE_H
#define INTERFACE_H

#include <QObject>

class ControlServer : public QObject
{
    Q_OBJECT
public:
    explicit ControlServer(QObject *parent = nullptr);

signals:
};

#endif // INTERFACE_H
