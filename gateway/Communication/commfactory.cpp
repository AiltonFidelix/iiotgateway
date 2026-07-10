#include "commfactory.hpp"

COMM_BEGIN_NAMESPACE

std::set<QByteArray> CommFactory::m_commInterfaces{};

CommInterface *CommFactory::getCommInterface(const QByteArray &commInterface, QJsonObject settings) {
    const QMetaType type = QMetaType::fromName(commInterface.toUpper());

    if (!m_commInterfaces.contains(commInterface) || !type.isRegistered()) {
        throw std::runtime_error("Comm Interface not found");
    }

    QObject *interfaceObject = type.metaObject()->newInstance(std::move(settings));

    if (interfaceObject == nullptr) {
        throw std::runtime_error("Failed to invoke Comm Interface");
    }

    return qobject_cast<CommInterface *>(interfaceObject);
}

COMM_END_NAMESPACE
