#ifndef COMMFACTORY_H
#define COMMFACTORY_H

#include <QJsonObject>
#include <expected>
#include <set>

#include "comm_global.hpp"
#include "comminterface.hpp"

COMM_BEGIN_NAMESPACE

enum class CommFactoryError : uint8_t {
    UNKNOWN = 0,
    INTERFACE_NOT_FOUND,
    FAILED_TO_CREATE,
};

class CommFactory {
public:
    CommFactory() = delete;

    static std::expected<CommInterface *, CommFactoryError> getCommInterface(const QByteArray &commInterface, QJsonObject settings = QJsonObject());

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
