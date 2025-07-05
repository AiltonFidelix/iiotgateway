#ifndef DBSTORAGE_H
#define DBSTORAGE_H

#include "storageinterface.h"
#include "dbconnection.h"

class DBStorage : public StorageInterface
{
    DBConnection *m_connection;

    bool insert(const QString &query);
    bool insertSettings(const QString &field, const QString &value);
    QSqlRecord selectSettings();
    QString currentDateTime();

public:
    DBStorage();
    ~DBStorage();

    bool verify() override;

    bool setActive(bool active) override;
    bool setCloudProtocol(const QString &protocol) override;
    bool setEdgeProtocol(const QString &protocol) override;
    bool setProtocolSettings(const QString &protocol, const QJsonObject &settings) override;

    bool active() override;
    QString cloudProtocol() override;
    QString edgeProtocol() override;
    QJsonObject protocolSettings(const QString &protocol) override;
    QPair<QString, QString> userCredentials() override;
};

#endif // DBSTORAGE_H
