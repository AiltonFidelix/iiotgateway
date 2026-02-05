#ifndef REBOOTFACTORY_H
#define REBOOTFACTORY_H

#include <functional>
#include <QByteArray>
#include <QHash>

#include "system_global.h"
#include "reboot.h"

SYSTEM_BEGIN_NAMESPACE

class RebootFactory
{
    using Creator = std::function<Reboot*()>;
    using RebootList = QHash<QString, Creator>;

    static RebootList *m_rebootList;

public:
    RebootFactory() = delete;

    static Reboot *create(const QString &platform);
    static int registerCreator(const QString &platform, Creator creator);
};

SYSTEM_END_NAMESPACE

#endif // REBOOTFACTORY_H
