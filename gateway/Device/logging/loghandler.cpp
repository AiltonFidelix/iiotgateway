#include "loghandler.h"

#include <syslog.h>
#include <iostream>
#include <map>
#include <unordered_map>

LOGGING_BEGIN_NAMESPACE

#ifdef QT_DEBUG
static int logLevel = LOG_DEBUG;
#else
static int logLevel = LOG_INFO;
#endif

static const std::unordered_map<QtMsgType, int> logPriorityMap {
    {QtDebugMsg, LOG_DEBUG},
    {QtInfoMsg, LOG_INFO},
    {QtWarningMsg, LOG_WARNING},
    {QtCriticalMsg, LOG_ERR},
    {QtFatalMsg, LOG_ERR},
};

static const std::unordered_map<int, std::string> logTagMap {
    {LOG_DEBUG, "[DEBUG]"},
    {LOG_INFO, "[INFO]"},
    {LOG_WARNING, "[WARNING]"},
    {LOG_ERR, "[ERROR]"},
};

LogHandler *LogHandler::instance()
{
    static LogHandler instance;
    return &instance;
}

LogHandler::LogHandler()
{
    const std::map<std::string, uint8_t> logLevelMap {
        {"deb", LOG_DEBUG},
        {"inf", LOG_INFO},
        {"war", LOG_WARNING},
        {"cri", LOG_ERR},
        {"fat", LOG_ERR},
    };

    const std::string logLevelConfig{qgetenv("LOG_LEVEL").toLower().toStdString()};

    auto it = logLevelMap.find(logLevelConfig);

    if (it != logLevelMap.end())
    {
        logLevel = it->second;
    }

    openlog(APP_NAME, LOG_CONS | LOG_NDELAY, LOG_LOCAL0);
}

LogHandler::~LogHandler()
{
    closelog();
}

void LogHandler::syslogHandler(QtMsgType type, const QMessageLogContext &context, const QString &message)
{
    auto itP = logPriorityMap.find(type);

    if (itP == logPriorityMap.end())
    {
        return;
    }

    const int logPriority = itP->second;

    if (logPriority > logLevel)
    {
        return;
    }

    auto itT = logTagMap.find(logPriority);

    const QString logTag{QString::fromStdString(itT->second)};

    QString identifier{context.function};
    identifier.remove('*');
    identifier.remove('&');

    static QRegularExpression regexp{QStringLiteral("\\w+.?(?=::)")};
    QRegularExpressionMatch className{regexp.match(identifier)};

    if (className.hasMatch())
    {
        identifier = className.captured(0);
        identifier.remove(')');
    }

    if (identifier.isEmpty())
    {
        identifier = QStringLiteral("Unknown");
    }

    const QString fmtMessage{QString("%0 - %1 - %2").arg(logTag, identifier, message)};
    const QByteArray outputMessage{fmtMessage.toUtf8()};

    syslog(logPriority, "%s", outputMessage.data());
    std::cout << outputMessage.data() << std::endl;
}

LOGGING_END_NAMESPACE
