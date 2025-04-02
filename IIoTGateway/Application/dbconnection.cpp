#include "dbconnection.h"

#include <QCoreApplication>
#include <QDir>
#include <QSqlError>
#include <QSqlQuery>

DBConnection *DBConnection::m_instance = nullptr;

DBConnection*
DBConnection::instance()
{
    if (m_instance == nullptr)
    {
        m_instance = new DBConnection();

        connect(qApp, &QCoreApplication::aboutToQuit, m_instance, &DBConnection::close);
    }

    return m_instance;
}

bool
DBConnection::open()
{
    auto dbType = qgetenv("DB_TYPE");
    auto dbName = qgetenv("DB_NAME");
    auto dbHost = qgetenv("DB_HOST");
    auto dbUser = qgetenv("DB_USER");
    auto dbPass = qgetenv("DB_PASS");

    auto ok = false;
    auto port = qEnvironmentVariableIntValue("DB_PORT", &ok);

    m_database = QSqlDatabase::addDatabase(dbType);
    m_database.setDatabaseName(dbName);

    if (dbType != "QSQLITE")
    {
        m_database.setHostName(dbHost);
        m_database.setUserName(dbUser);
        m_database.setPassword(dbPass);
        m_database.setPort(ok ? port : -1);
    }

    return m_database.open();
}

void
DBConnection::close()
{
    m_database.close();
}

bool
DBConnection::isOpen()
{
    return m_database.isOpen();
}

bool
DBConnection::verifyScripts()
{
    auto insertHistory = [](const QString &script) -> bool
    {
        QSqlQuery query;
        return (!query.exec(QString("INSERT INTO history(id, script) VALUES(NULL, '%1')").arg(script)));
    };

    auto scriptExec = [this](const QString &fileName) -> bool
    {
        QFile file(fileName);

        if (!file.open(QIODevice::ReadOnly))
            return false;

        const auto data = file.readAll();
        const auto scripts = data.trimmed().split(';');

        file.close();

        m_database.transaction();

        auto ok = true;

        for (const auto &script : scripts)
        {
            if (script.isEmpty())
                continue;

            QSqlQuery query;

            const auto ret = query.exec(script);

            if (!ret)
            {
                qWarning() << query.lastError().text();
            }

            ok &= ret;
        }

        ok ? m_database.commit() : m_database.rollback();

        return ok;
    };

    auto scriptExists = [](const QString &script) -> bool
    {
        QSqlQuery query;

        if (!query.exec(QString("SELECT count(script) FROM history WHERE script = '%1'").arg(script)))
        {
            qWarning() << query.lastError().text();
            return false;
        }

        if (!query.next())
            return false;

        return (query.value(0).toInt() == 1);
    };

    auto tableExists = [](const QString &table) -> bool
    {
        QSqlQuery query;

        if (!query.exec(QString("SELECT count(name) FROM sqlite_master WHERE type = 'table' AND name = '%1'").arg(table)))
            return false;

        if (!query.next())
            return false;

        return (query.value(0).toInt() == 1);
    };

    const QString historyScript = "history.sql";

    QDir dir(":/scripts");

    if (!dir.exists())
        return false;

    auto files = dir.entryList(QDir::Files);

    if (!files.contains(historyScript))
        return false;

    files.removeOne(historyScript);

    if (!tableExists("history"))
    {
        qDebug() << "Executing" << historyScript;

        if (!scriptExec(QString("%1/%2").arg(dir.absolutePath(), historyScript)))
        {
            qWarning() << "Failed to execute" << historyScript;
            return false;
        }

        insertHistory(historyScript);
    }

    auto ok = true;

    for (const auto &fileName : std::as_const(files))
    {
        if (!scriptExists(fileName))
        {
            qDebug() << "Executing" << fileName;

            if (!scriptExec(QString("%1/%2").arg(dir.absolutePath(), fileName)))
            {
                qWarning() << "Failed to execute" << fileName;
                ok = false;
            }
            else
            {
                insertHistory(fileName);
            }
        }
    }

    return ok;
}

QString
DBConnection::lastError() const
{
    return m_database.lastError().text();
}
