#ifndef DBSTORAGE_H
#define DBSTORAGE_H

#include "storage.h"
#include "dbconnection.h"

class DBStorage : public Storage
{
public:
    DBStorage();
    ~DBStorage();

    bool verify() override;

    bool setActive(bool active) override;
    bool setCloudProtocol(const QString &protocol) override;
    bool setEdgeProtocol(const QString &protocol) override;
    bool setProtocolSettings(const QString &protocol, const QJsonDocument &settings) override;

    bool active() override;
    QString cloudProtocol() override;
    QString edgeProtocol() override;
    QJsonDocument protocolSettings(const QString &protocol) override;

private:
    bool settingsExist();
    bool insert(const QString &query);
    bool insertSettings(const QString &field, const QString &value);
    QSqlRecord selectSettings();
    QString currentDateTime();

private:
    DBConnection *m_connection;
};

#endif // DBSTORAGE_H
