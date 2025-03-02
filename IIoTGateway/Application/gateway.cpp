#include "gateway.h"

#include <QDebug>

#include "../Protocols/commfactory.h"

Gateway::Gateway(QObject *parent)
    : QObject(parent),
    m_threadEdge(nullptr),
    m_threadCloud(nullptr)
{
    qDebug() << "Creating the gateway object ihul...";
}

Gateway::~Gateway()
{
    stop();
}

void
Gateway::start()
{
    qDebug() << "Starting gateway...";

    CommFactory commFactory;

    QByteArray edgeProtocol = qgetenv("EDGE_PROTOCOL").toUpper();
    QByteArray cloudProtocol = qgetenv("CLOUD_PROTOCOL").toUpper();

    try
    {
        m_threadEdge = new QThread();
        m_threadEdge->setObjectName(edgeProtocol);

        CommInterface *commEdge = commFactory.getCommInterface(edgeProtocol);
        commEdge->moveToThread(m_threadEdge);

        m_threadCloud = new QThread();
        m_threadCloud->setObjectName(cloudProtocol);

        CommInterface *commCloud = commFactory.getCommInterface(cloudProtocol);
        commCloud->moveToThread(m_threadCloud);

        connect(commEdge, &CommInterface::outgoing, commCloud, &CommInterface::incoming, Qt::QueuedConnection);
        connect(commCloud, &CommInterface::outgoing, commEdge, &CommInterface::incoming, Qt::QueuedConnection);

        connect(m_threadEdge, &QThread::started, commEdge, &CommInterface::connectComm);
        connect(m_threadCloud, &QThread::started, commCloud, &CommInterface::connectComm);

        connect(m_threadEdge, &QThread::finished, commEdge, &CommInterface::disconnectComm);
        connect(commEdge, &CommInterface::disconnected, commEdge, &CommInterface::deleteLater);
        connect(commEdge, &CommInterface::error, this, &Gateway::notifyError, Qt::QueuedConnection);
        connect(commEdge, &CommInterface::connectionFailed, this, &Gateway::stop, Qt::QueuedConnection);

        connect(m_threadCloud, &QThread::finished, commCloud, &CommInterface::disconnectComm);
        connect(commCloud, &CommInterface::disconnected, commCloud, &CommInterface::deleteLater);
        connect(commCloud, &CommInterface::error, this, &Gateway::notifyError, Qt::QueuedConnection);
        connect(commCloud, &CommInterface::connectionFailed, this, &Gateway::stop, Qt::QueuedConnection);

        m_threadEdge->start();
        m_threadCloud->start();
    }
    catch (std::exception &e)
    {
        notifyError(e.what());
    }
}

void
Gateway::stop()
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

    qDebug() << "Stoping gateway...";
}

void
Gateway::restart()
{
    qDebug() << "Restarting gateway...";
    stop();
    start();
}

void
Gateway::notifyError(QByteArray error)
{
    qWarning() << error;
}
