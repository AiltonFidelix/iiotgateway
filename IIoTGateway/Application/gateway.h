#ifndef GATEWAY_H
#define GATEWAY_H

#include <QObject>

#include "../Protocols/comminterface.h"

class Gateway : public QObject
{
    Q_OBJECT
public:
    explicit Gateway(QObject *parent = nullptr);
    ~Gateway() = default;

    void start();

signals:

private:
    CommInterface *m_edge;
    CommInterface *m_cloud;
};

#endif // GATEWAY_H
