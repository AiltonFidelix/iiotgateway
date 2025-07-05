#ifndef COMMFACTORY_H
#define COMMFACTORY_H

#include <QObject>
#include <QSet>
#include <QJsonObject>

#include "comm_global.h"
#include "comminterface.h"

COMM_BEGIN_NAMESPACE

class CommFactory : public QObject
{
    Q_OBJECT

    static QSet<QByteArray> *m_commInterfaces;

public:
    CommFactory() = delete;

    static QSet<QByteArray> *commInterfaces();

    static CommInterface *getCommInterface(const QByteArray &commInterface, QJsonObject settings = QJsonObject{});

    template<typename T> static int registerInterface(QByteArray commInterface)
    {
        if (m_commInterfaces == nullptr)
        {
            m_commInterfaces = new QSet<QByteArray>{};
        }

        m_commInterfaces->insert(commInterface);

        return qRegisterMetaType<T>(commInterface);
    }
};

COMM_END_NAMESPACE

#endif // COMMFACTORY_H
