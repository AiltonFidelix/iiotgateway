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
        _commInterfaces.insert(commInterface);

        return qRegisterMetaType<T>(commInterface);
    }

private:
    static std::set<QByteArray> _commInterfaces;
};

COMM_END_NAMESPACE

#endif  // COMMFACTORY_H
