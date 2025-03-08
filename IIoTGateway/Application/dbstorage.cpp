#include "dbstorage.h"

#include <QDateTime>
#include <QSqlQuery>
#include <QSqlRecord>

DBStorage::DBStorage()
{
    m_connection = DBConnection::instance();
}

DBStorage::~DBStorage()
{
    m_connection->close();
}

bool
DBStorage::verify()
{
    if (!m_connection->open())
    {
        qFatal() << "Database connection failed:" << m_connection->lastError();
        return false;
    }

    return m_connection->verifyScripts();
}

bool
DBStorage::setActive(bool active)
{
    QString value = active ? "Y" : "N";
    return insertSettings("active", value);
}

bool
DBStorage::setCloudProtocol(const QString &protocol)
{
    return insertSettings("cloud_protocol", protocol);
}

bool
DBStorage::setEdgeProtocol(const QString &protocol)
{
    return insertSettings("edge_protocol", protocol);
}

bool
DBStorage::setProtocolSettings(const QString &protocol, const QJsonDocument &settings)
{
    auto currentDT = currentDateTime();
    auto strSettings = settings.toJson(QJsonDocument::Compact);

    QString query = QString("INSERT INTO protocols (settings, updated) VALUES ('%1', '%2') WHERE type = '%3'")
                        .arg(strSettings, currentDT, protocol);

    auto ret = insert(query);

    if (!ret)
    {
        query = QString("INSERT INTO protocols (id, type, settings, updated) VALUES (NULL, '%1', '%2', '%3', '%4')")
                    .arg(protocol, strSettings, currentDT, currentDT);

        ret = insert(query);
    }

    return ret;
}

bool
DBStorage::active()
{
    auto record = selectSettings();

    if (record.isEmpty())
        return false;

    auto index = static_cast<int>(DBConnection::Settings::Active);

    return record.value(index).toString() == "Y";
}

QString
DBStorage::cloudProtocol()
{
    auto record = selectSettings();

    if (record.isEmpty())
        return QString();

    auto index = static_cast<int>(DBConnection::Settings::CloudProtocol);

    return record.value(index).toString();
}

QString
DBStorage::edgeProtocol()
{
    auto record = selectSettings();

    if (record.isEmpty())
        return QString();

    auto index = static_cast<int>(DBConnection::Settings::EdgeProtocol);

    return record.value(index).toString();
}

QJsonDocument
DBStorage::protocolSettings(const QString &protocol)
{
    return QJsonDocument();
}

bool
DBStorage::settingsExist()
{
    QSqlQuery sqlquery;

    auto ret = sqlquery.exec("SELECT count(*) FROM settings");

    ret &= sqlquery.next();
    ret &= sqlquery.value(0).toInt() == 1;

    return ret;
}

bool
DBStorage::insert(const QString &query)
{
    QSqlQuery sqlquery;
    return sqlquery.exec(query);
}

bool
DBStorage::insertSettings(const QString &field, const QString &value)
{
    auto currentDT = currentDateTime();
    auto ret = false;

    if (settingsExist())
    {
        ret = insert(QString("UPDATE settings SET %1 = '%2', updated = '%3' WHERE id = 1")
                         .arg(field, value, currentDT));
    }
    else
    {
        ret = insert(QString("INSERT INTO settings (id, %1, created, updated) VALUES (NULL, '%2', '%3', '%4')")
                         .arg(field, value, currentDT, currentDT));
    }

    return ret;
}

QSqlRecord
DBStorage::selectSettings()
{
    QSqlQuery sqlquery;

    if (!sqlquery.exec("SELECT * FROM settings") || !sqlquery.next())
    {
        return QSqlRecord();
    }

    return sqlquery.record();
}

QString
DBStorage::currentDateTime()
{
    return QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss");
}
