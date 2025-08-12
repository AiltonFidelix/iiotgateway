#include "gateway.h"

#include <QDebug>

#include "commfactory.h"

using comm::CommFactory;

Gateway::Gateway(StorageInterface *storage, QObject *parent)
    : QObject(parent),
    m_isRunning(false),
    m_threadEdge(nullptr),
    m_threadCloud(nullptr),
    m_storage(storage)
{
    qInfo() << "Creating gateway instance...";
}

Gateway::~Gateway()
{
    if (m_isRunning)
        stop();
}

bool Gateway::isRunning() const
{
    return m_isRunning;
}

bool Gateway::start()
{
    qDebug() << "Starting gateway...";

    if (m_storage == nullptr)
    {
        qWarning() << "Failed to start: No settings storage!";
        return false;
    }

    const QByteArray cloudProtocol = m_storage->cloudProtocol().toUtf8();
    const QByteArray edgeProtocol = m_storage->edgeProtocol().toUtf8();

    const QJsonObject settingsCloudProtocol = m_storage->protocolSettings(cloudProtocol);
    const QJsonObject settingsEdgeProtocol = m_storage->protocolSettings(edgeProtocol);

    if (cloudProtocol.isEmpty() || edgeProtocol.isEmpty())
    {
        qWarning() << "Failed to start: Missing Protocol configuration!";
        return false;
    }

    try
    {
        m_threadCloud = new QThread();
        m_threadCloud->setObjectName(cloudProtocol);

        CommInterface *commCloud = CommFactory::getCommInterface(cloudProtocol, settingsCloudProtocol);
        commCloud->moveToThread(m_threadCloud);

        m_threadEdge = new QThread();
        m_threadEdge->setObjectName(edgeProtocol);

        CommInterface *commEdge = CommFactory::getCommInterface(edgeProtocol, settingsEdgeProtocol);
        commEdge->moveToThread(m_threadEdge);

        connect(commCloud, &CommInterface::outgoing, commEdge, &CommInterface::incoming, Qt::QueuedConnection);
        connect(commEdge, &CommInterface::outgoing, commCloud, &CommInterface::incoming, Qt::QueuedConnection);

        connect(m_threadCloud, &QThread::started, commCloud, &CommInterface::connectComm);
        connect(m_threadEdge, &QThread::started, commEdge, &CommInterface::connectComm);

        connect(m_threadCloud, &QThread::finished, commCloud, &CommInterface::disconnectComm);
        connect(commCloud, &CommInterface::disconnected, commCloud, &CommInterface::deleteLater);
        connect(commCloud, &CommInterface::error, this, &Gateway::notifyError, Qt::QueuedConnection);
        connect(commCloud, &CommInterface::connectionFailed, this, &Gateway::stop, Qt::QueuedConnection);

        connect(m_threadEdge, &QThread::finished, commEdge, &CommInterface::disconnectComm);
        connect(commEdge, &CommInterface::disconnected, commEdge, &CommInterface::deleteLater);
        connect(commEdge, &CommInterface::error, this, &Gateway::notifyError, Qt::QueuedConnection);
        connect(commEdge, &CommInterface::connectionFailed, this, &Gateway::stop, Qt::QueuedConnection);

        m_threadCloud->start();
        m_threadEdge->start();

        m_isRunning = true;
        m_storage->setActive(m_isRunning);
    }
    catch (std::exception &e)
    {
        notifyError(e.what());
        return false;
    }

    return true;
}

void Gateway::stop()
{
    auto quitThread = [](QThread *thread) -> QThread*
    {
        if (thread != nullptr)
        {
            if (thread->isRunning())
            {
                thread->quit();
                thread->wait();
            }

            delete thread;
            thread = nullptr;
        }

        return thread;
    };

    m_threadEdge = quitThread(m_threadEdge);
    m_threadCloud = quitThread(m_threadCloud);

    m_isRunning = false;
    m_storage->setActive(m_isRunning);

    qDebug() << "Stoping gateway...";
}

bool Gateway::restart()
{
    qDebug() << "Restarting gateway...";
    stop();
    return start();
}

void Gateway::notifyError(const QByteArray &error)
{
    qWarning() << "Error notification:" << error;
}
