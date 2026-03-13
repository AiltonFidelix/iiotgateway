#ifndef LOGHANDLER_H
#define LOGHANDLER_H

#include "logging_global.h"

#include <QRegularExpression>

LOGGING_BEGIN_NAMESPACE

class LogHandler
{
    LogHandler();
    ~LogHandler();

public:
    static LogHandler *instance();
    static void syslogHandler(QtMsgType type, const QMessageLogContext &context, const QString &message);
};

LOGGING_END_NAMESPACE

#endif // LOGHANDLER_H
