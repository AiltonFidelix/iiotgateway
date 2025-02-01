#include "commmqtt.h"
#include "commfactory.h"

#include <QDebug>
#include <QJsonDocument>

int CommMQTT::m_typeId = CommFactory::registerInterface<CommMQTT*>("MQTT");

bool
CommMQTT::isconnected()
{
    return true;
}

void
CommMQTT::connectComm()
{
    qDebug() << Q_FUNC_INFO;
}

void
CommMQTT::disconnectComm()
{
    qDebug() << Q_FUNC_INFO;
}

void
CommMQTT::incoming(QByteArray data)
{
    QJsonDocument json = QJsonDocument::fromJson(data);
    qDebug() << Q_FUNC_INFO << json;
}
