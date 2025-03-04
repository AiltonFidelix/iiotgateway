#ifndef DBCONNECTION_H
#define DBCONNECTION_H

#include <QObject>
#include <QSqlDatabase>

class DBConnection : public QObject
{
    Q_OBJECT
public:
    static DBConnection *instance();

    bool isOpen();

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
