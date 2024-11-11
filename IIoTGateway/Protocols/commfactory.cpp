#include "commfactory.h"

QSet<QByteArray>* CommFactory::m_commInterfaces;

QSet<QByteArray>*
CommFactory::commInterfaces()
{
    return m_commInterfaces;
}

CommInterface*
CommFactory::getCommInterface(const QByteArray &commInterface)
{
    QMetaType type = QMetaType::fromName(commInterface.toUpper());

    if (!m_commInterfaces->contains(commInterface) || !type.isRegistered())
    {
        throw std::runtime_error("Comm Interface not found");
    }

    QObject *interfaceObject = type.metaObject()->newInstance();

    if (interfaceObject == nullptr)
    {
        throw std::runtime_error("Failed to invoke Comm Interface");
    }

    CommInterface *interface = qobject_cast<CommInterface*>(interfaceObject);

    return interface;
}
