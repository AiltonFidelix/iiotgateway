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

    bool isRunning();

public slots:
    void start();
    void stop();
    void restart();

private slots:
    void notifyError(QByteArray error);

private:
    bool m_isRunning;
    QThread *m_threadEdge;
    QThread *m_threadCloud;
};

#endif // GATEWAY_H
