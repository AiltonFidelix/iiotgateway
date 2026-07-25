#include "dbconnection.h"

#include <QCoreApplication>
#include <QDir>
#include <QSqlError>
#include <QSqlQuery>

DBConnection *DBConnection::_instance = nullptr;

DBConnection *
DBConnection::instance() {
    if (_instance == nullptr) {
        _instance = new DBConnection();

        connect(qApp, &QCoreApplication::aboutToQuit, _instance, &DBConnection::close);
    }

    return _instance;
}

bool DBConnection::open() {
    const QByteArray dbType = qgetenv("DB_TYPE");
    const QByteArray dbName = qgetenv("DB_NAME");
    const QByteArray dbHost = qgetenv("DB_HOST");
    const QByteArray dbUser = qgetenv("DB_USER");
    const QByteArray dbPass = qgetenv("DB_PASS");

    bool ok = false;
    const int port = qEnvironmentVariableIntValue("DB_PORT", &ok);

    _database = QSqlDatabase::addDatabase(dbType);
    _database.setDatabaseName(dbName);

    if (dbType != "QSQLITE") {
        _database.setHostName(dbHost);
        _database.setUserName(dbUser);
        _database.setPassword(dbPass);
        _database.setPort(ok ? port : -1);
    }

    qInfo() << "Opening connection..";

    return _database.open();
}

void DBConnection::close() {
    qInfo() << "Closing connection..";
    _database.close();
}

bool DBConnection::isOpen() const {
    return _database.isOpen();
}

bool DBConnection::verifyScripts() {
    auto insertHistory = [](const QString &script) -> bool {
        QSqlQuery query;
        return (!query.exec(QString("INSERT INTO history(id, script) VALUES(NULL, '%1')").arg(script)));
    };

    auto scriptExec = [this](const QString &fileName) -> bool {
        QFile file(fileName);

        if (!file.open(QIODevice::ReadOnly)) {
            return false;
        }

        const QByteArray data = file.readAll();
        const auto scripts = data.trimmed().split(';');

        file.close();

        bool ok = _database.transaction();

        if (!ok) {
            return false;
        }

        for (const QByteArray &script : scripts) {
            if (script.isEmpty())
                continue;

            QSqlQuery query;

            const bool ret = query.exec(script);

            if (!ret) {
                qWarning() << "Query failed:" << query.lastError().text();
            }

            ok &= ret;
        }

        ok ? _database.commit() : _database.rollback();

        return ok;
    };

    auto scriptExists = [](const QString &script) -> bool {
        QSqlQuery query;

        if (!query.exec(QString("SELECT count(script) FROM history WHERE script = '%1'").arg(script))) {
            qWarning() << "Query failed:" << query.lastError().text();
            return false;
        }

        if (!query.next()) {
            return false;
        }

        return (query.value(0).toInt() == 1);
    };

    auto tableExists = [](const QString &table) -> bool {
        QSqlQuery query;

        if (!query.exec(QString("SELECT count(name) FROM sqlite_master WHERE type = 'table' AND name = '%1'").arg(table))) {
            return false;
        }

        if (!query.next()) {
            return false;
        }

        return (query.value(0).toInt() == 1);
    };

    const QString historyScript(QStringLiteral("history.sql"));

    QDir dir(QStringLiteral(":/scripts"));

    if (!dir.exists()) {
        return false;
    }

    auto files = dir.entryList(QDir::Files);

    if (!files.contains(historyScript)) {
        return false;
    }

    files.removeOne(historyScript);

    if (!tableExists(QStringLiteral("history"))) {
        qDebug() << "Executing script:" << historyScript;

        if (!scriptExec(QString("%1/%2").arg(dir.absolutePath(), historyScript))) {
            qWarning() << "Failed to execute script:" << historyScript;
            return false;
        }

        insertHistory(historyScript);
    }

    bool ok = true;

    for (const auto &fileName : std::as_const(files)) {
        if (!scriptExists(fileName)) {
            qDebug() << "Executing script:" << fileName;

            if (!scriptExec(QString("%1/%2").arg(dir.absolutePath(), fileName))) {
                qWarning() << "Failed to execute script:" << fileName;
                ok = false;
            } else {
                insertHistory(fileName);
            }
        }
    }

    return ok;
}

QString DBConnection::lastError() const {
    return _database.lastError().text();
}
