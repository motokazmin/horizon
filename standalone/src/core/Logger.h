#pragma once

#include <QString>
#include <QDateTime>
#include <QFile>
#include <QTextStream>
#include <QMutex>

namespace HorizonUTM {

enum class LogLevel {
    Debug,
    Info,
    Warning,
    Error,
    Critical
};

class Logger {
public:
    static Logger& instance();
    
    void log(LogLevel level, const QString& message, const QString& source = "");
    
    void debug(const QString& message, const QString& source = "");
    void info(const QString& message, const QString& source = "");
    void warning(const QString& message, const QString& source = "");
    void error(const QString& message, const QString& source = "");
    void critical(const QString& message, const QString& source = "");
    
    void setLogFile(const QString& filePath);
    void setLogLevel(LogLevel level);
    
private:
    Logger();
    ~Logger();
    Logger(const Logger&) = delete;
    Logger& operator=(const Logger&) = delete;
    
    QString levelToString(LogLevel level) const;
    void writeToFile(const QString& formattedMessage);
    
    QFile m_logFile;
    QTextStream m_stream;
    QMutex m_mutex;
    LogLevel m_minLevel;
};

// Convenience macros
#define LOG_DEBUG(msg) HorizonUTM::Logger::instance().debug(msg, __FUNCTION__)
#define LOG_INFO(msg) HorizonUTM::Logger::instance().info(msg, __FUNCTION__)
#define LOG_WARNING(msg) HorizonUTM::Logger::instance().warning(msg, __FUNCTION__)
#define LOG_ERROR(msg) HorizonUTM::Logger::instance().error(msg, __FUNCTION__)
#define LOG_CRITICAL(msg) HorizonUTM::Logger::instance().critical(msg, __FUNCTION__)

} // namespace HorizonUTM
