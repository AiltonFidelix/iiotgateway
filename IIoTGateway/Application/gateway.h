#ifndef GATEWAY_H
#define GATEWAY_H

#include <QObject>

class Gateway : public QObject
{
    Q_OBJECT
public:
    explicit Gateway(QObject *parent = nullptr);
    ~Gateway() = default;

signals:
};

#endif // GATEWAY_H
