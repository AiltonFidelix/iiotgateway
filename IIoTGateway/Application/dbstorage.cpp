#include "dbstorage.h"

#include <QDateTime>
#include <QSqlError>
#include <QSqlQuery>
#include <QSqlRecord>

#include <QJsonDocument>

DBStorage::DBStorage()
    : m_connection{DBConnection::instance()}
{
}

DBStorage::~DBStorage()
{
    m_connection->close();
}

bool DBStorage::verify()
{
    if (!m_connection->open())
    {
        qFatal() << "Database connection failed:" << m_connection->lastError();
        return false;
    }

    return m_connection->verifyScripts();
}

bool DBStorage::setActive(bool active)
{
    const QString value = active ? "Y" : "N";
    return insertSettings("active", value);
}

bool DBStorage::setCloudProtocol(const QString &protocol)
{
    return insertSettings("cloud_protocol", protocol);
}

bool DBStorage::setEdgeProtocol(const QString &protocol)
{
    return insertSettings("edge_protocol", protocol);
}

bool DBStorage::setProtocolSettings(const QString &protocol, const QJsonObject &settings)
{
    auto settingsExist = [](const QString &protocol) -> bool
    {
        QSqlQuery sqlquery;

        bool ret = sqlquery.exec(QString("SELECT count(*) FROM protocols WHERE type = '%1'").arg(protocol));

        ret &= sqlquery.next();
        ret &= sqlquery.value(0).toInt() == 1;

        return ret;
    };

    const QString currentDT{currentDateTime()};

    auto strSettings = QJsonDocument{settings}.toJson(QJsonDocument::Compact);

    bool ret = false;

    if (settingsExist(protocol))
    {
        ret = insert(QString("UPDATE protocols SET settings = '%1', updated = '%2' WHERE type = '%3'")
                         .arg(strSettings, currentDT, protocol));
    }
    else
    {
        ret = insert(QString("INSERT INTO protocols (id, type, settings, created, updated) VALUES (NULL, '%1', '%2', '%3', '%4')")
                         .arg(protocol, strSettings, currentDT, currentDT));
    }

    return ret;
}

bool DBStorage::active()
{
    QSqlRecord record = selectSettings();

    if (record.isEmpty())
        return false;

    auto index = static_cast<int>(DBConnection::Settings::Active);

    return record.value(index).toString() == "Y";
}

QString DBStorage::cloudProtocol()
{
    QSqlRecord record = selectSettings();

    if (record.isEmpty())
        return QString{};

    auto index = static_cast<int>(DBConnection::Settings::CloudProtocol);

    return record.value(index).toString();
}

QString DBStorage::edgeProtocol()
{
    QSqlRecord record = selectSettings();

    if (record.isEmpty())
        return QString{};

    auto index = static_cast<int>(DBConnection::Settings::EdgeProtocol);

    return record.value(index).toString();
}

QJsonObject DBStorage::protocolSettings(const QString &protocol)
{
    QSqlQuery sqlquery;

    bool ret = sqlquery.prepare(QString("SELECT settings FROM protocols WHERE type = '%1'").arg(protocol));

    ret &= sqlquery.exec();

    if (!ret)
        qWarning() << sqlquery.lastError().text();

    ret &= sqlquery.next();

    if (!ret)
        return QJsonObject{};

    const auto settings = sqlquery.value(0).toByteArray();
    const auto doc = QJsonDocument::fromJson(settings);

    return doc.object();
}

QPair<QString, QString> DBStorage::userCredentials()
{
    QSqlQuery sqlquery;

    bool ret = sqlquery.prepare("SELECT username, password FROM users WHERE id = 1");

    ret &= sqlquery.exec();

    if (!ret)
        qWarning() << sqlquery.lastError().text();

    ret &= sqlquery.next();

    if (!ret)
        return QPair<QString, QString>{};

    const QPair<QString, QString> credentials{sqlquery.value(0).toString(), sqlquery.value(1).toString()};

    return credentials;
}

bool DBStorage::insert(const QString &query)
{
    QSqlQuery sqlquery;
    return sqlquery.exec(query);
}

bool DBStorage::insertSettings(const QString &field, const QString &value)
{
    auto settingsExist = []() -> bool
    {
        QSqlQuery sqlquery;

        bool ret = sqlquery.exec("SELECT count(*) FROM settings");

        ret &= sqlquery.next();
        ret &= sqlquery.value(0).toInt() == 1;

        return ret;
    };

    const QString currentDT = currentDateTime();
    bool ret = false;

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

QSqlRecord DBStorage::selectSettings()
{
    QSqlQuery sqlquery;

    if (!sqlquery.exec("SELECT * FROM settings") || !sqlquery.next())
    {
        return QSqlRecord{};
    }

    return sqlquery.record();
}

QString DBStorage::currentDateTime()
{
    return QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss");
}
