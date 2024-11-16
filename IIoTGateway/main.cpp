#include <QCoreApplication>
#include "Application/gateway.h"

int main(int argc, char *argv[])
{
    QCoreApplication app(argc, argv);

    Gateway gateway;
    gateway.start();

    // qDebug() << QCoreApplication::translate("IIoTGateway", "(Press CTRL+C to quit)");

    return app.exec();
}
