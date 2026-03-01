#ifndef LOGHANDLER_H
#define LOGHANDLER_H

#include "logging_global.h"

#include <QRegularExpression>

LOGGING_BEGIN_NAMESPACE

class LogHandler
{
    LogHandler();
    ~LogHandler();

    using PriorityMap = QHash<QtMsgType, QPair<int, QString>>;
    using PriorityMapIterator = QHashIterator<QtMsgType, QPair<int, QString>>;

    static LogHandler *m_instance;
    static PriorityMap m_priorityMap;
    static int m_logLevel;
    static int m_syslogLevel;

public:
    static LogHandler *instance();

    static void syslogHandler(QtMsgType type, const QMessageLogContext &context, const QString &msg);
};

LOGGING_END_NAMESPACE

#endif // LOGHANDLER_H
