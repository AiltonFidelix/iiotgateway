#ifndef DBSTORAGE_H
#define DBSTORAGE_H

#include "dbconnection.h"
#include "storageinterface.h"

class DBStorage final : public StorageInterface {
public:
    DBStorage();
    ~DBStorage();

    bool verify() const override;

    bool setActive(bool active) const override;
    bool setCloudProtocol(const QString &protocol) const override;
    bool setEdgeProtocol(const QString &protocol) const override;
    bool setProtocolSettings(const QString &protocol, const QJsonObject &settings) const override;

    bool active() const override;
    QString cloudProtocol() const override;
    QString edgeProtocol() const override;
    QJsonObject protocolSettings(const QString &protocol) const override;
    QPair<QString, QString> userCredentials() const override;

private:
    bool insert(const QString &query) const;
    bool insertSettings(const QString &field, const QString &value) const;
    QSqlRecord selectSettings() const;
    QString currentDateTime() const;

    DBConnection *_connection;
};

#endif  // DBSTORAGE_H
