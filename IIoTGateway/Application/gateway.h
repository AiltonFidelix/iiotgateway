#ifndef GATEWAY_H
#define GATEWAY_H

#include <QObject>
#include <QThread>

class Gateway : public QObject
{
    Q_OBJECT
public:
    explicit Gateway(QObject *parent = nullptr);
    ~Gateway();

public slots:
    void start();
    void stop();

signals:

private:
    // CommInterface *m_commEdge;
    // CommInterface *m_commCloud;

    QThread *m_threadEdge;
    QThread *m_threadCloud;
};

#endif // GATEWAY_H
