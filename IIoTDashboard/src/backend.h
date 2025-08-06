#ifndef BACKEND_H
#define BACKEND_H

#include <QObject>
#include <QNetworkAccessManager>
#include <QNetworkReply>

class Backend : public QObject
{
    Q_OBJECT

    QNetworkAccessManager m_manager;
    QString m_serverUrl;

    void sendCommand(const QString &command);
    QString errorHandler(QNetworkReply::NetworkError e);

public:
    explicit Backend(QObject *parent = nullptr);

signals:
    void settings(QByteArray settings);
    void status(QString status);
    void success(QString message);
    void error(QString error);

public slots:
    void login(const QString &username, const QString &password);
    void setSettings(const QByteArray &settings);
    void requestSettings(const QStringList &protocols);
    void requestStatus();
    void start();
    void stop();
    void restart();
};

#endif // BACKEND_H
