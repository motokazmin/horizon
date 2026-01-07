#include "Logger.h"
#include <QDebug>
#include <QStandardPaths>
#include <QDir>

namespace HorizonUTM {

Logger::Logger() 
    : m_minLevel(LogLevel::Debug)
{
    // Create logs directory
    QString logDir = QStandardPaths::writableLocation(QStandardPaths::AppDataLocation) + "/logs";
    QDir().mkpath(logDir);
    
    // Open log file
    QString logFilePath = logDir + QString("/horizon_utm_%1.log")
        .arg(QDateTime::currentDateTime().toString("yyyyMMdd_HHmmss"));
    
    m_logFile.setFileName(logFilePath);
    if (m_logFile.open(QIODevice::WriteOnly | QIODevice::Append | QIODevice::Text)) {
        m_stream.setDevice(&m_logFile);
    }
}

Logger::~Logger() {
    if (m_logFile.isOpen()) {
        m_logFile.close();
    }
}

void Logger::initialize(LogLevel level, const QString& logFile) {
    Logger& logger = instance();
    logger.setLogLevel(level);
    if (!logFile.isEmpty()) {
        logger.setLogFile(logFile);
    }
}

Logger& Logger::instance() {
    static Logger instance;
    return instance;
}

void Logger::log(LogLevel level, const QString& message, const QString& source) {
    if (level < m_minLevel) {
        return;
    }
    
    QMutexLocker locker(&m_mutex);
    
    QString timestamp = QDateTime::currentDateTime().toString("yyyy-MM-dd HH:mm:ss.zzz");
    QString levelStr = levelToString(level);
    QString sourceStr = source.isEmpty() ? "" : QString(" [%1]").arg(source);
    
    QString formattedMessage = QString("%1 [%2]%3: %4")
        .arg(timestamp)
        .arg(levelStr)
        .arg(sourceStr)
        .arg(message);
    
    // Output to console
    qDebug().noquote() << formattedMessage;
    
    // Write to file
    writeToFile(formattedMessage);
}

void Logger::debug(const QString& message, const QString& source) {
    log(LogLevel::Debug, message, source);
}

void Logger::info(const QString& message, const QString& source) {
    log(LogLevel::Info, message, source);
}

void Logger::warning(const QString& message, const QString& source) {
    log(LogLevel::Warning, message, source);
}

void Logger::error(const QString& message, const QString& source) {
    log(LogLevel::Error, message, source);
}

void Logger::critical(const QString& message, const QString& source) {
    log(LogLevel::Critical, message, source);
}

void Logger::setLogFile(const QString& filePath) {
    QMutexLocker locker(&m_mutex);
    
    if (m_logFile.isOpen()) {
        m_logFile.close();
    }
    
    m_logFile.setFileName(filePath);
    if (m_logFile.open(QIODevice::WriteOnly | QIODevice::Append | QIODevice::Text)) {
        m_stream.setDevice(&m_logFile);
    }
}

void Logger::setLogLevel(LogLevel level) {
    m_minLevel = level;
}

QString Logger::levelToString(LogLevel level) const {
    switch (level) {
        case LogLevel::Debug:    return "DEBUG";
        case LogLevel::Info:     return "INFO";
        case LogLevel::Warning:  return "WARNING";
        case LogLevel::Error:    return "ERROR";
        case LogLevel::Critical: return "CRITICAL";
        default:                 return "UNKNOWN";
    }
}

void Logger::writeToFile(const QString& formattedMessage) {
    if (m_logFile.isOpen()) {
        m_stream << formattedMessage << Qt::endl;
        m_stream.flush();
    }
}

} // namespace HorizonUTM
