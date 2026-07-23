#ifndef BACKEND_H
#define BACKEND_H

#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QObject>

class Backend : public QObject {
    Q_OBJECT
public:
    explicit Backend(QObject *parent = nullptr);

signals:
    void communicationSettings(QByteArray settings);
    void networkSettings(QByteArray settings);
    void status(QString status);
    void success(QString message);
    void error(QString error);

public slots:
    void login(const QString &username, const QString &password);
    void setCommunicationSettings(const QByteArray &settings);
    void requestCommunicationSettings(const QStringList &protocols);
    void setNetworkSettings(const QByteArray &settings);
    void requestNetworkSettings();
    void requestStatus();
    void start();
    void stop();
    void restart();
    void reboot();

private:
    QPair<bool, QString> parserReply(QNetworkReply *reply) const;
    void sendCommand(const QString &command);
    QString errorHandler(QNetworkReply::NetworkError e);

    QNetworkAccessManager _manager;
    QString _serverUrl;
};

#endif  // BACKEND_H
