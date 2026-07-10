#ifndef COMMFACTORY_H
#define COMMFACTORY_H

#include <QJsonObject>
#include <set>

#include "comm_global.hpp"
#include "comminterface.hpp"

COMM_BEGIN_NAMESPACE

class CommFactory {
public:
    CommFactory() = delete;

    static CommInterface *getCommInterface(const QByteArray &commInterface, QJsonObject settings = QJsonObject());

    template <typename T>
    static int registerInterface(QByteArray commInterface) {
        m_commInterfaces.insert(commInterface);

        return qRegisterMetaType<T>(commInterface);
    }

private:
    static std::set<QByteArray> m_commInterfaces;
};

COMM_END_NAMESPACE

#endif  // COMMFACTORY_H
