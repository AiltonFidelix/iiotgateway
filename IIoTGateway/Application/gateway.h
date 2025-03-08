#ifndef GATEWAY_H
#define GATEWAY_H

#include <QObject>
#include <QThread>

#include "storage.h"

class Gateway : public QObject
{
    Q_OBJECT
public:
    explicit Gateway(Storage *storage = nullptr, QObject *parent = nullptr);
    ~Gateway();

    bool isRunning();

public slots:
    bool start();
    void stop();
    bool restart();

private slots:
    void notifyError(QByteArray error);

private:
    bool m_isRunning;
    QThread *m_threadEdge;
    QThread *m_threadCloud;
    Storage *m_storage;
};

#endif // GATEWAY_H
