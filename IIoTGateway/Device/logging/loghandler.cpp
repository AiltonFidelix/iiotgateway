#include "loghandler.h"

#include <syslog.h>
#include <iostream>
#include <QDateTime>
#include <QMetaEnum>
#include <QThread>

LOGGING_BEGIN_NAMESPACE

LogHandler *LogHandler::m_instance = nullptr;
LogHandler::PriorityMap LogHandler::m_priorityMap = LogHandler::PriorityMap();
int LogHandler::m_logLevel = LOG_DEBUG;
int LogHandler::m_syslogLevel = LOG_DEBUG;

LogHandler *LogHandler::instance()
{
    if (m_instance == nullptr)
        m_instance = new LogHandler();

    return m_instance;
}

LogHandler::LogHandler()
{
#ifdef QT_DEBUG
    m_logLevel = LOG_DEBUG;
#else
    m_logLevel = LOG_INFO;
#endif

    m_priorityMap = {
        { QtDebugMsg, qMakePair(LOG_DEBUG, QStringLiteral("[DEBUG]")) },
        { QtInfoMsg, qMakePair(LOG_INFO, QStringLiteral("[INFO]")) },
        { QtWarningMsg, qMakePair(LOG_WARNING, QStringLiteral("[WARNING]")) },
        { QtCriticalMsg, qMakePair(LOG_ERR, QStringLiteral("[ERROR]")) },
        { QtFatalMsg, qMakePair(LOG_ERR, QStringLiteral("[ERROR]")) }
    };

    QString logLevel = qgetenv("LOG_LEVEL").toLower();
    QString syslogLevel = qgetenv("LOG_LEVEL_FOR_SYSLOG_FILES").toLower();
    PriorityMapIterator iPriorityMap(m_priorityMap);

    while (iPriorityMap.hasNext())
    {
        iPriorityMap.next();

        if (logLevel.contains(iPriorityMap.value().second.toLower()))
            m_logLevel = iPriorityMap.value().first;

        if (syslogLevel.contains(iPriorityMap.value().second.toLower()))
            m_syslogLevel = iPriorityMap.value().first;
    }

    openlog(APP_NAME, LOG_CONS | LOG_NDELAY, LOG_LOCAL0);
}

LogHandler::~LogHandler()
{
    closelog();
}

void
LogHandler::syslogHandler(QtMsgType type, const QMessageLogContext &context, const QString &msg)
{
    QStringList messageInfo;
    QString messagType = QStringLiteral("DEB");
    QString identificador = context.function;
    identificador.remove('*');
    identificador.remove('&');

    static QRegularExpression regexp(QStringLiteral("\\w+.?(?=::)"));
    QRegularExpressionMatch className = regexp.match(identificador);

    if (className.hasMatch())
    {
        identificador = className.captured(0);
    }

    if (m_priorityMap.contains(type))
    {
        messagType = m_priorityMap.value(type).second;
    }

    const QVariant messageContext = QThread::currentThread()->property("context");

    if (messageContext.isValid())
    {
        identificador = messageContext.toString();
    }
    else if (identificador.isEmpty())
    {
        identificador = QStringLiteral("Unknown");
    }

    messageInfo.append(QDateTime::currentDateTime().toString(QStringLiteral("dd-MM-yyyy hh:mm:ss")));
    messageInfo.append(identificador);
    messageInfo.append(msg);

    const QByteArray separator(" - ");
    const QString outputMessage1 = messageInfo.join(separator);
    QByteArray outputMessage = outputMessage1.toUtf8();

    if (m_priorityMap.value(type).first <= m_syslogLevel)
    {
        syslog(m_priorityMap.value(type).first, "%s", outputMessage.data());
    }

    messageInfo.prepend(messagType);
    outputMessage = messageInfo.join(separator).toUtf8();

    if (m_priorityMap.value(type).first <= m_logLevel)
    {
        std::cout << outputMessage.data() << std::endl;
    }
}

LOGGING_END_NAMESPACE
