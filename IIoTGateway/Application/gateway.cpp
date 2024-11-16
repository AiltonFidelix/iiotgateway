#include "gateway.h"

#include <QDebug>

#include "../Protocols/commfactory.h"

Gateway::Gateway(QObject *parent)
    : QObject{parent}
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

        connect(m_threadEdge, &QThread::finished, commEdge, &CommInterface::deleteLater);
        // connect(m_threadCloud, &QThread::finished, commCloud, &CommInterface::deleteLater);

        m_threadEdge->start();
    }
    catch (std::exception &e)
    {
        qWarning() << e.what();
        stop();
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
}
