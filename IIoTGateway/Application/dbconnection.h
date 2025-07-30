#ifndef DBCONNECTION_H
#define DBCONNECTION_H

#include <QObject>
#include <QSqlDatabase>

class DBConnection : public QObject
{
    Q_OBJECT

    static DBConnection *m_instance;
    QSqlDatabase m_database;

    DBConnection() = default;
    ~DBConnection() = default;

public:

    // History table enum
    enum class History : uint8_t {
        Id = 0,
        Script,
        Created
    };

    // Settings table enum
    enum class Settings : uint8_t {
        Id = 0,
        Active,
        CloudProtocol,
        EdgeProtocol,
        Created,
        Updated
    };

    // Protocols table enum
    enum class Protocols : uint8_t {
        Id = 0,
        Type,
        Settings,
        Created,
        Updated
    };

    static DBConnection *instance();

    bool isOpen() const;
    bool verifyScripts();

    QString lastError() const;

public slots:
    bool open();
    void close();
};

#endif // DBCONNECTION_H
