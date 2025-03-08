#include <QCoreApplication>
#include <signal.h>

#include "control.h"
#include "dbstorage.h"
#include "gateway.h"

static Gateway *gateway = nullptr;

Q_NORETURN void quit(int sig)
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
        qCritical("Received %s", sigName.data());
    }

    qDebug("Closing all connections and exiting the process...");

    if (gateway)
    {
        gateway->stop();
        gateway->deleteLater();
    }

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

int main(int argc, char *argv[])
{
    QCoreApplication app(argc, argv);
    app.setApplicationName(APP_NAME);

    setup_unix_signal_handlers();

    DBStorage dbstorage;

    if (!dbstorage.verify())
    {
        qFatal() << "Failed to verify storage!";
        return -1;
    }

    Control control;
    // control.start();

    gateway = new Gateway(&dbstorage);
    gateway->start();

    return app.exec();
}
