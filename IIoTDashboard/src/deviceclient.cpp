#include "deviceclient.h"

#include <QDebug>

#ifdef Q_OS_WASM
#include <emscripten/emscripten.h>
#include <emscripten/val.h>

EM_JS(emscripten::EM_VAL, getServerUrl, (), {
    let url = window.location.protocol + "//" + window.location.hostname;
    return Emval.toHandle(url);
});
#endif

DeviceClient::DeviceClient(QObject *parent)
    : QObject{parent}
{
#ifdef Q_OS_WASM
    auto url = emscripten::val::take_ownership(getServerUrl());
    m_serverUrl = QString::fromStdString(url.as<std::string>());
#else
    m_serverUrl = "http://localhost";
#endif

    qDebug() << "Server Url:" << m_serverUrl;
}

void
DeviceClient::setDeviceSettings(QByteArray settings)
{
    qDebug() << "New settings:" << settings;

    emit success("Settings successfully saved!");
}

