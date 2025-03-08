#ifndef DBCONNECTION_H
#define DBCONNECTION_H

#include <QObject>
#include <QSqlDatabase>

class DBConnection : public QObject
{
    Q_OBJECT
public:

    // History table enum
    enum class History {
        Id = 0,
        Script,
        Created
    };

    // Settings table enum
    enum class Settings {
        Id = 0,
        Active,
        CloudProtocol,
        EdgeProtocol,
        Created,
        Updated
    };

    // Protocols table enum
    enum class Protocols {
        Id = 0,
        Type,
        Settings,
        Created,
        Updated
    };

    static DBConnection *instance();

    bool isOpen();
    bool verifyScripts();

    QString lastError() const;

public slots:
    bool open();
    void close();

private:
    DBConnection() = default;
    ~DBConnection() = default;

private:
    static DBConnection *m_instance;
    QSqlDatabase m_database;
};

#endif // DBCONNECTION_H
