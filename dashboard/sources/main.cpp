#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>

#include "backend.h"

int main(int argc, char *argv[]) {
    QGuiApplication app(argc, argv);

    QLocale::setDefault(QLocale::C);

    QQmlApplicationEngine engine;

    const QUrl url(QStringLiteral("qrc:/ui/Main.qml"));

    // TODO: Create a control class for each page
    Backend backend;
    engine.rootContext()->setContextProperty(QStringLiteral("backend"), &backend);

    QObject::connect(
        &engine,
        &QQmlApplicationEngine::objectCreated,
        &app,
        [url](const QObject *obj, const QUrl &objUrl) {
            if (!obj && url == objUrl)
                QCoreApplication::exit(-1);
        },
        Qt::QueuedConnection);

    engine.load(url);

    return app.exec();
}
