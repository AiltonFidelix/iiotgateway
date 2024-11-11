#ifndef COMMFACTORY_H
#define COMMFACTORY_H

#include <QObject>
#include <QSet>

#include "comminterface.h"

class CommFactory : public QObject
{
    Q_OBJECT
public:
    CommFactory() = default;
    virtual ~CommFactory() = default;

    static QSet<QByteArray> *commInterfaces();

    CommInterface *getCommInterface(const QByteArray &commInterface);

    template<typename T> static int registerInterface(QByteArray commInterface)
    {
        if (m_commInterfaces == nullptr)
        {
            m_commInterfaces = new QSet<QByteArray>();
        }

        m_commInterfaces->insert(commInterface);

        return qRegisterMetaType<T>(commInterface);
    }

private:
    static QSet<QByteArray>* m_commInterfaces;
};

#endif // COMMFACTORY_H
