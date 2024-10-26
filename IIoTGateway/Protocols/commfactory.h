#ifndef COMMFACTORY_H
#define COMMFACTORY_H

#include <QObject>

class CommFactory : public QObject
{
    Q_OBJECT
public:
    explicit CommFactory(QObject *parent = nullptr);

signals:
};

#endif // COMMFACTORY_H
