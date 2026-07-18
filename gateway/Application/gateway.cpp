#include "gateway.hpp"

#include <QDebug>

#include "commfactory.hpp"

using comm::CommFactory;

Gateway::Gateway(StorageInterface *storage, QObject *parent)
    : QObject{parent},
      _isRunning{false},
      _threadEdge{nullptr},
      _threadCloud{nullptr},
      _storage{storage} {
    qInfo() << "Creating gateway instance...";
}

Gateway::~Gateway() {
    if (_isRunning) {
        stop();
    }
}

bool Gateway::isRunning() const {
    return _isRunning;
}

bool Gateway::start() {
    qInfo() << "Starting gateway...";

    if (_storage == nullptr) {
        qWarning() << "Failed to start: No settings storage!";
        return false;
    }

    const QByteArray cloudProtocol = _storage->cloudProtocol().toUtf8();
    const QByteArray edgeProtocol = _storage->edgeProtocol().toUtf8();

    QJsonObject settingsCloudProtocol = _storage->protocolSettings(cloudProtocol);
    QJsonObject settingsEdgeProtocol = _storage->protocolSettings(edgeProtocol);

    if (cloudProtocol.isEmpty() || edgeProtocol.isEmpty()) {
        qWarning() << "Failed to start: Missing Protocol configuration!";
        return false;
    }

    try {
        _threadCloud = new QThread();
        _threadCloud->setObjectName(cloudProtocol);

        CommInterface *commCloud = CommFactory::getCommInterface(cloudProtocol, std::move(settingsCloudProtocol));
        commCloud->moveToThread(_threadCloud);

        _threadEdge = new QThread();
        _threadEdge->setObjectName(edgeProtocol);

        CommInterface *commEdge = CommFactory::getCommInterface(edgeProtocol, std::move(settingsEdgeProtocol));
        commEdge->moveToThread(_threadEdge);

        connect(commCloud, &CommInterface::outgoing, commEdge, &CommInterface::incoming, Qt::QueuedConnection);
        connect(commEdge, &CommInterface::outgoing, commCloud, &CommInterface::incoming, Qt::QueuedConnection);

        connect(_threadCloud, &QThread::started, commCloud, &CommInterface::connectComm);
        connect(_threadEdge, &QThread::started, commEdge, &CommInterface::connectComm);

        connect(_threadCloud, &QThread::finished, commCloud, &CommInterface::disconnectComm);
        connect(commCloud, &CommInterface::disconnected, commCloud, &CommInterface::deleteLater);
        connect(commCloud, &CommInterface::error, this, &Gateway::notifyError, Qt::QueuedConnection);
        connect(commCloud, &CommInterface::connectionFailed, this, &Gateway::stop, Qt::QueuedConnection);

        connect(_threadEdge, &QThread::finished, commEdge, &CommInterface::disconnectComm);
        connect(commEdge, &CommInterface::disconnected, commEdge, &CommInterface::deleteLater);
        connect(commEdge, &CommInterface::error, this, &Gateway::notifyError, Qt::QueuedConnection);
        connect(commEdge, &CommInterface::connectionFailed, this, &Gateway::stop, Qt::QueuedConnection);

        _threadCloud->start();
        _threadEdge->start();

        _isRunning = true;
        _storage->setActive(_isRunning);
    } catch (std::exception &e) {
        notifyError(e.what());
        return false;
    }

    return true;
}

void Gateway::stop() {
    auto quitThread = [](QThread *thread) -> QThread * {
        if (thread != nullptr) {
            if (thread->isRunning()) {
                thread->quit();
                thread->wait();
            }

            delete thread;
            thread = nullptr;
        }

        return thread;
    };

    _threadEdge = quitThread(_threadEdge);
    _threadCloud = quitThread(_threadCloud);

    _isRunning = false;
    _storage->setActive(_isRunning);

    qInfo() << "Stoping gateway...";
}

bool Gateway::restart() {
    qInfo() << "Restarting gateway...";
    stop();
    return start();
}

void Gateway::notifyError(const QByteArray &error) {
    qWarning() << "Error notification:" << error;
}
