#include "commfactory.h"

COMM_BEGIN_NAMESPACE

QSet<QByteArray> *CommFactory::m_commInterfaces = nullptr;

QSet<QByteArray> *CommFactory::commInterfaces()
{
    return m_commInterfaces;
}

CommInterface *CommFactory::getCommInterface(const QByteArray &commInterface, const QJsonObject &settings)
{
    const QMetaType type = QMetaType::fromName(commInterface.toUpper());

    if (!m_commInterfaces->contains(commInterface) || !type.isRegistered())
    {
        throw std::runtime_error("Comm Interface not found");
    }

    QObject *interfaceObject = type.metaObject()->newInstance(settings);

    if (interfaceObject == nullptr)
    {
        throw std::runtime_error("Failed to invoke Comm Interface");
    }

    return qobject_cast<CommInterface*>(interfaceObject);;
}

COMM_END_NAMESPACE
