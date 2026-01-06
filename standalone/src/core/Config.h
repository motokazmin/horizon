#pragma once

#include <QString>
#include <QSettings>
#include <memory>

namespace HorizonUTM {

class Config {
public:
    static Config& instance();
    
    // Database
    QString getDatabasePath() const;
    void setDatabasePath(const QString& path);
    
    // Hardware
    QString getLastUsedPort() const;
    void setLastUsedPort(const QString& port);
    
    double getDefaultSpeed() const;
    void setDefaultSpeed(double speed);
    
    // UI
    bool isDarkTheme() const;
    void setDarkTheme(bool enabled);
    
    // Test Defaults
    QString getDefaultTestMethod() const;
    void setDefaultTestMethod(const QString& method);
    
    double getDefaultGaugeLength() const;
    void setDefaultGaugeLength(double length);
    
    // Paths
    QString getExportPath() const;
    void setExportPath(const QString& path);
    
    QString getAppDataPath() const;
    
private:
    Config();
    ~Config() = default;
    Config(const Config&) = delete;
    Config& operator=(const Config&) = delete;
    
    std::unique_ptr<QSettings> m_settings;
};

} // namespace HorizonUTM
