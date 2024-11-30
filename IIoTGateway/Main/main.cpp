#include <QCoreApplication>
#include "gateway.h"

int main(int argc, char *argv[])
{
    QCoreApplication app(argc, argv);

    Gateway gateway;
    gateway.start();

    // qDebug() << QCoreApplication::translate("IIoTGateway", "(Press CTRL+C to quit)");

    return app.exec();
}
