#include <QCoreApplication>
#include <iostream>
#include "gateway.h"

int main(int argc, char *argv[])
{
    QCoreApplication app(argc, argv);

    Gateway gateway;
    gateway.start();

    // qDebug() << QCoreApplication::translate("IIoTGateway", "(Press CTRL+C to quit)");

    // std::cout << "IIoTGateway (Press q to quit)\n";
    // char input;

    // do
    // {
    //     std::cin >> input;
    // } while ((std::toupper(input) != 'q') && (std::toupper(input) != 'Q'));

    // return 0;

    return app.exec();
}
