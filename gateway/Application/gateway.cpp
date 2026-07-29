#include "gateway.hpp"

#include <QDebug>

#include "commfactory.hpp"

using comm::CommFactory;
using comm::CommFactoryError;

// TODO Create restart routine
// TODO Detect when some protocol disconnect and enter restart routine

Gateway::Gateway(StorageInterface *storage, QObject *parent)
    : QObject{parent},
      _isRunning{false},
      _storage{storage},
      _cloudThread{nullptr},
      _edgeThread{nullptr} {
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

    if (cloudProtocol.isEmpty() || edgeProtocol.isEmpty()) {
        qWarning() << "Failed to start: Missing protocol configuration!";
        return false;
    }

    QJsonObject cloudSettings = _storage->protocolSettings(cloudProtocol);
    QJsonObject edgeSettings = _storage->protocolSettings(edgeProtocol);

    if (cloudSettings.isEmpty() || edgeSettings.isEmpty()) {
        qWarning() << "Failed to start: Missing protocol settings!";
        return false;
    }

    CommInterface *cloudComm = createCommunication(cloudProtocol, std::move(cloudSettings));
    CommInterface *edgeComm = createCommunication(edgeProtocol, std::move(edgeSettings));

    if ((cloudComm == nullptr) || (edgeComm == nullptr)) {
        qWarning() << "Failed to start: Was not able to create the communications!";
        return false;
    }

    _cloudThread.reset(new QThread());
    _edgeThread.reset(new QThread());

    if ((_cloudThread == nullptr) || (_edgeThread == nullptr)) {
        qWarning() << "Failed to start: Was not able to create communication threads!";
        return false;
    }

    _cloudThread->setObjectName(cloudProtocol);
    _edgeThread->setObjectName(edgeProtocol);

    if (!cloudComm->moveToThread(_cloudThread.get()) || !edgeComm->moveToThread(_edgeThread.get())) {
        qWarning() << "Failed to start: Was not able to create communication threads!";
        return false;
    }

    connect(cloudComm, &CommInterface::outgoing, edgeComm, &CommInterface::incoming, Qt::QueuedConnection);
    connect(edgeComm, &CommInterface::outgoing, cloudComm, &CommInterface::incoming, Qt::QueuedConnection);

    connect(_cloudThread.get(), &QThread::started, cloudComm, &CommInterface::connectComm);
    connect(_edgeThread.get(), &QThread::started, edgeComm, &CommInterface::connectComm);

    connect(_cloudThread.get(), &QThread::finished, cloudComm, &CommInterface::disconnectComm);
    connect(cloudComm, &CommInterface::disconnected, cloudComm, &CommInterface::deleteLater);
    connect(cloudComm, &CommInterface::error, this, &Gateway::notifyError, Qt::QueuedConnection);
    connect(cloudComm, &CommInterface::connectionFailed, this, &Gateway::stop, Qt::QueuedConnection);

    connect(_edgeThread.get(), &QThread::finished, edgeComm, &CommInterface::disconnectComm);
    connect(edgeComm, &CommInterface::disconnected, edgeComm, &CommInterface::deleteLater);
    connect(edgeComm, &CommInterface::error, this, &Gateway::notifyError, Qt::QueuedConnection);
    connect(edgeComm, &CommInterface::connectionFailed, this, &Gateway::stop, Qt::QueuedConnection);

    _cloudThread->start();
    _edgeThread->start();

    _isRunning = true;
    _storage->setActive(_isRunning);

    return true;
}

void Gateway::stop() {
    _cloudThread.reset();
    _edgeThread.reset();

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

CommInterface *Gateway::createCommunication(const QByteArray &protocol, QJsonObject settings) const {
    auto handleError = [&](const CommFactoryError error) -> void {
        switch (error) {
            case CommFactoryError::INTERFACE_NOT_FOUND:
                qWarning() << protocol << "interface not found";
                break;
            case CommFactoryError::FAILED_TO_CREATE:
                qWarning() << "Failed to create" << protocol << "communication";
                break;
            default:
                qWarning() << "Undefined error";
                break;
        }
    };

    std::expected<CommInterface *, CommFactoryError> commReturn = CommFactory::getCommInterface(protocol, std::move(settings));

    if (!commReturn.has_value()) {
        handleError(commReturn.error());
        return nullptr;
    }

    return commReturn.value();
}

void CommThreadDeleter::operator()(QThread *thread) {
    if (thread == nullptr) {
        return;
    }

    if (thread->isRunning()) {
        thread->quit();
        thread->wait();
    }

    delete thread;
    thread = nullptr;
}
