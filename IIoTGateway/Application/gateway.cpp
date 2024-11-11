#include "gateway.h"

#include <QDebug>

#include "../Protocols/commfactory.h"

Gateway::Gateway(QObject *parent)
    : QObject{parent}
{
    qDebug() << "Creating the gateway object ihul...";

    start();
}

void
Gateway::start()
{
    CommFactory commFactory;

    QByteArray edgeProtocol = qgetenv("EDGE_PROTOCOL");
    QByteArray cloudProtocol = qgetenv("CLOUD_PROTOCOL");

    try
    {
        m_edge = commFactory.getCommInterface(edgeProtocol);

        m_edge->connectComm();

        m_cloud = commFactory.getCommInterface(cloudProtocol);
    }
    catch (std::exception &e)
    {
        qWarning() << e.what();
    }
}
