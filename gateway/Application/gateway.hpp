#ifndef GATEWAY_H
#define GATEWAY_H

#include <QObject>
#include <QThread>
#include <memory>

#include "comminterface.hpp"
#include "storageinterface.h"

struct CommThreadDeleter {
    void operator()(QThread *thread);
};

class Gateway : public QObject {
    Q_OBJECT
public:
    explicit Gateway(StorageInterface *storage = nullptr, QObject *parent = nullptr);
    ~Gateway();

    Gateway(const Gateway &other) = delete;
    Gateway operator==(const Gateway &other) = delete;

    bool isRunning() const;

public slots:
    bool start();
    void stop();
    bool restart();

private slots:
    void notifyError(const QByteArray &error);

private:
    CommInterface *createCommunication(const QByteArray &protocol, QJsonObject settings) const;

    bool _isRunning;
    StorageInterface *_storage;
    std::unique_ptr<QThread, CommThreadDeleter> _cloudThread;
    std::unique_ptr<QThread, CommThreadDeleter> _edgeThread;
};

#endif  // GATEWAY_H
