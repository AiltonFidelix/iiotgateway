#include "gateway.h"

#include <QDebug>

Gateway::Gateway(QObject *parent)
    : QObject{parent}
{
    qDebug() << "Creating the gateway object ihul...";
}
