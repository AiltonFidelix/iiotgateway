#include <signal.h>

#include <QCoreApplication>
#include <array>
#include <map>

#include "control.h"
#include "dbstorage.h"
#include "gateway.hpp"
#include "logging/loghandler.h"

using device::logging::LogHandler;

static Control *control = nullptr;

[[noreturn]] void quit(int sig) {
    static const std::map<int, std::string> sigNames{
        {SIGINT, "SIGINT"},
        {SIGTERM, "SIGTERM"},
        {SIGKILL, "SIGKILL"},
        {SIGQUIT, "SIGQUIT"},
        {SIGHUP, "SIGHUP"},
        {SIGINT, "SIGINT"},
    };

    if (auto it = sigNames.find(sig); it != sigNames.end()) {
        qCritical() << "Received:" << *it;
    }

    qDebug() << "Closing all connections and exiting the process...";

    if (control) {
        control->stop();
        delete control;
    }

    exit(EXIT_FAILURE);
}

void setup_unix_signal_handlers() {
    constexpr std::array<int, 7> sigs{SIGINT, SIGABRT, SIGTERM, SIGKILL, SIGQUIT, SIGHUP, SIGINT};

    for (const int &sig : sigs) {
        signal(sig, quit);
    }
}

int main(int argc, char *argv[]) {
    QCoreApplication app(argc, argv);
    app.setApplicationName(APP_NAME);

    setup_unix_signal_handlers();

    auto logHandler = LogHandler::instance();
    qInstallMessageHandler(logHandler->syslogHandler);

    DBStorage dbstorage;

    if (!dbstorage.verify()) {
        qFatal() << "Failed to verify storage!";
        return -1;
    }

    const QString platform = qEnvironmentVariable("PLATFORM", QStringLiteral("host"));

    qDebug() << "PLATFORM:" << platform;

    Gateway gateway(&dbstorage);

    control = new Control(platform);

    control->setGateway(&gateway);
    control->setStorage(&dbstorage);
    control->start();

    return app.exec();
}
