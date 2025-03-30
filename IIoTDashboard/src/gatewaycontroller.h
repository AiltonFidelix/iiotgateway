#ifndef GATEWAYCONTROLLER_H
#define GATEWAYCONTROLLER_H

#include <QObject>
#include <QNetworkAccessManager>
#include <QNetworkReply>

class GatewayController : public QObject
{
    Q_OBJECT
public:
    explicit GatewayController(QObject *parent = nullptr);

signals:
    void settings(QByteArray settings);
    void status(QByteArray status);
    void success(QByteArray message);
    void error(QByteArray error);

public slots:
    void setSettings(const QByteArray &settings);
    void requestSettings();
    void requestStatus();
    void start();
    void stop();
    void restart();

private:
    void sendCommand(const QString &command);

private slots:
    void errorHandler(QNetworkReply::NetworkError e);

private:
    QNetworkAccessManager m_manager;
    QString m_serverUrl;
};

#endif // GATEWAYCONTROLLER_H
