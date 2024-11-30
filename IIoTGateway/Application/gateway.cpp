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
    CommFactory commFactory;

    QByteArray edgeProtocol = qgetenv("EDGE_PROTOCOL");
    QByteArray cloudProtocol = qgetenv("CLOUD_PROTOCOL");

    try
    {
        m_threadEdge = new QThread();
        m_threadEdge->setObjectName(edgeProtocol);

        CommInterface *commEdge = commFactory.getCommInterface(edgeProtocol);
        commEdge->moveToThread(m_threadEdge);

        // m_threadCloud = new QThread();
        // CommInterface *commCloud = commFactory.getCommInterface(cloudProtocol);
        // commCloud->moveToThread(m_threadCloud);

        // connect(commEdge, &CommInterface::outgoing, commCloud, &CommInterface::incoming, Qt::QueuedConnection);
        // connect(commCloud, &CommInterface::outgoing, commEdge, &CommInterface::incoming, Qt::QueuedConnection);

        connect(m_threadEdge, &QThread::started, commEdge, &CommInterface::connectComm);
        // connect(m_threadCloud, &QThread::started, commCloud, &CommInterface::connectComm);

        connect(m_threadEdge, &QThread::finished, commEdge, &CommInterface::disconnectComm);
        connect(commEdge, &CommInterface::disconnected, commEdge, &CommInterface::deleteLater);
        connect(commEdge, &CommInterface::error, this, &Gateway::notifyError, Qt::QueuedConnection);
        // connect(m_threadCloud, &QThread::finished, commCloud, &CommInterface::deleteLater);

        m_threadEdge->start();
    }
    catch (std::exception &e)
    {
        notifyError(e.what());
    }
}

void
Gateway::stop()
{
    if (m_threadEdge)
    {
        m_threadEdge->quit();
        m_threadEdge->wait();
        delete m_threadEdge;
    }

    if (m_threadCloud)
    {
        m_threadCloud->quit();
        m_threadCloud->wait();
        delete m_threadCloud;
    }

    m_threadEdge = nullptr;
    m_threadCloud = nullptr;
}

void
Gateway::notifyError(QByteArray error)
{
    qWarning() << error;
    stop();
}
