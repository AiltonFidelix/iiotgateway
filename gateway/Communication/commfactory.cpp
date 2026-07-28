#include "commfactory.hpp"

COMM_BEGIN_NAMESPACE

std::set<QByteArray> CommFactory::_commInterfaces{};

std::expected<CommInterface *, CommFactoryError> CommFactory::getCommInterface(const QByteArray &commInterface, QJsonObject settings) {
    const QMetaType type = QMetaType::fromName(commInterface.toUpper());

    if (!_commInterfaces.contains(commInterface) || !type.isRegistered()) {
        return std::unexpected(CommFactoryError::INTERFACE_NOT_FOUND);
    }

    QObject *interfaceObject = type.metaObject()->newInstance(std::move(settings));

    if (interfaceObject == nullptr) {
        return std::unexpected(CommFactoryError::FAILED_TO_CREATE);
    }

    return qobject_cast<CommInterface *>(interfaceObject);
}

COMM_END_NAMESPACE
