#include "dbconnection.h"

#include <QCoreApplication>
#include <QSqlError>

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

QString
DBConnection::lastError() const
{
    return m_database.lastError().text();
}
