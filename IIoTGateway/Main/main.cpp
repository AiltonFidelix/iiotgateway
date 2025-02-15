#include <QCoreApplication>
#include <signal.h>
#include <iostream>
#include "main.h"
#include "gateway.h"

static Gateway *gateway = nullptr;

int main(int argc, char *argv[])
{
    QCoreApplication app(argc, argv);
    app.setApplicationName(APP_NAME);

    setup_unix_signal_handlers();

    gateway = new Gateway();
    gateway->start();

    return app.exec();
}

[[ noreturn ]] void quit(int sig)
{
    const QHash<int, QByteArray> sigNames = {
        {SIGINT, "SIGINT"},
        {SIGTERM, "SIGTERM"},
        {SIGKILL, "SIGKILL"},
        {SIGQUIT, "SIGQUIT"},
        {SIGHUP, "SIGHUP"},
        {SIGINT, "SIGINT"}
    };

    if (sigNames.contains(sig))
    {
        auto sigName = sigNames.value(sig);
        qWarning("Received %s", sigName.data());
    }

    qDebug("Closing all connections and exiting the process...");

    gateway->stop();
    gateway->deleteLater();

    exit(EXIT_FAILURE);
}

void setup_unix_signal_handlers()
{
    const QSet<int> sigs {SIGINT, SIGABRT, SIGTERM, SIGKILL, SIGQUIT, SIGHUP, SIGINT};

    for (int sig : sigs)
    {
        signal(sig, quit);
    }
}
