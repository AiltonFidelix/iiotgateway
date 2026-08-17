#include "gateway.hpp"

#include <QDebug>
#include <QEventLoop>

#include "commfactory.hpp"

using comm::CommFactory;
using comm::CommFactoryError;

namespace {
    constexpr int COMM_ERROR_CODE = 1;
}

// TODO Create restart routine
// TODO Detect when some protocol disconnect and enter restart routine

Gateway::Gateway(StorageInterface *storage, QObject *parent)
    : QObject{parent},
      _isRunning{false},
      _storage{storage},
      _cloudThread{nullptr},
      _edgeThread{nullptr},
      _mutex{} {
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
        qWarning() << "No settings storage!";
        return false;
    }

    const QByteArray edgeProtocol = _storage->edgeProtocol().toUtf8();
    const QByteArray cloudProtocol = _storage->cloudProtocol().toUtf8();

    if (edgeProtocol.isEmpty() || cloudProtocol.isEmpty()) {
        qWarning() << "Missing protocol configuration!";
        return false;
    }

    QJsonObject edgeSettings = _storage->protocolSettings(edgeProtocol);
    QJsonObject cloudSettings = _storage->protocolSettings(cloudProtocol);

    if (edgeSettings.isEmpty() || cloudSettings.isEmpty()) {
        qWarning() << "Missing protocol settings!";
        return false;
    }

    CommInterface *edgeComm = createCommunication(edgeProtocol, std::move(edgeSettings));
    CommInterface *cloudComm = createCommunication(cloudProtocol, std::move(cloudSettings));

    if ((edgeComm == nullptr) || (cloudComm == nullptr)) {
        qWarning() << "Was not able to create the communications!";
        return false;
    }

    _edgeThread.reset(new QThread());
    _cloudThread.reset(new QThread());

    if ((_edgeThread == nullptr) || (_cloudThread == nullptr)) {
        qWarning() << "Was not able to create communication threads!";
        return false;
    }

    _edgeThread->setObjectName(edgeProtocol);
    _cloudThread->setObjectName(cloudProtocol);

    if (!edgeComm->moveToThread(_edgeThread.get()) || !cloudComm->moveToThread(_cloudThread.get())) {
        qWarning() << "Was not able to create communication threads!";
        return false;
    }

    connect(_edgeThread.get(), &QThread::started, edgeComm, &CommInterface::connectComm);
    connect(_cloudThread.get(), &QThread::started, cloudComm, &CommInterface::connectComm);

    connect(_edgeThread.get(), &QThread::finished, edgeComm, &CommInterface::disconnectComm);
    connect(edgeComm, &CommInterface::disconnected, edgeComm, &CommInterface::deleteLater);

    connect(_cloudThread.get(), &QThread::finished, cloudComm, &CommInterface::disconnectComm);
    connect(cloudComm, &CommInterface::disconnected, cloudComm, &CommInterface::deleteLater);

    if (!startCommunicationThread(_edgeThread, edgeComm) || !startCommunicationThread(_cloudThread, cloudComm)) {
        return false;
    }

    connect(edgeComm, &CommInterface::connectionFailed, this, &Gateway::stop, Qt::QueuedConnection);
    connect(cloudComm, &CommInterface::connectionFailed, this, &Gateway::stop, Qt::QueuedConnection);

    connect(edgeComm, &CommInterface::outgoing, cloudComm, &CommInterface::incoming, Qt::QueuedConnection);
    connect(cloudComm, &CommInterface::outgoing, edgeComm, &CommInterface::incoming, Qt::QueuedConnection);

    qInfo() << "Gateway started!";

    _isRunning = true;
    _storage->setActive(_isRunning);

    return true;
}

void Gateway::stop() {
    qInfo() << "Stoping gateway...";

    if (!_mutex.try_lock()) {
        qDebug() << "Stop already called from one of the communication threads, just returning";
        return;
    }

    _cloudThread.reset();
    _edgeThread.reset();

    _isRunning = false;

    _storage->setActive(_isRunning);

    _mutex.unlock();
}

bool Gateway::restart() {
    qInfo() << "Restarting gateway...";
    stop();
    return start();
}

bool Gateway::startCommunicationThread(std::unique_ptr<QThread, CommThreadDeleter> &commThread, CommInterface *comm) const {
    qInfo().noquote() << "Starting" << commThread->objectName() << "communication thread...";

    QEventLoop loop;

    QObject::connect(comm, &CommInterface::connected, &loop, &QEventLoop::quit, Qt::QueuedConnection);
    QObject::connect(comm, &CommInterface::connectionFailed, &loop, &QEventLoop::exit, Qt::QueuedConnection);

    commThread->start();

    const int returnCode = loop.exec();

    qDebug() << "[returnCode]" << returnCode;

    if (returnCode == COMM_ERROR_CODE) {
        qWarning().noquote() << "Could not start" << commThread->objectName() << "communication thread!";
        return false;
    }

    qInfo().noquote() << commThread->objectName() << "communication thread started!";

    return true;
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

    qDebug().noquote() << "Deleting [thread]" << thread->objectName();

    if (thread->isRunning()) {
        thread->quit();
        thread->wait();
    }

    delete thread;
    thread = nullptr;
}
