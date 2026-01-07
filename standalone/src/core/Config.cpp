#include "Config.h"
#include "Constants.h"
#include <QStandardPaths>
#include <QDir>

namespace HorizonUTM {

Config::Config() {
    m_settings = std::make_unique<QSettings>(
        Constants::APP_ORGANIZATION,
        Constants::APP_NAME
    );
}

Config& Config::instance() {
    static Config instance;
    return instance;
}

void Config::load() {
    // Settings are loaded automatically by QSettings
    m_settings->sync();
}

void Config::save() {
    m_settings->sync();
}

QString Config::getDatabasePath() const {
    QString defaultPath = getAppDataPath() + "/" + Constants::DB_NAME;
    return m_settings->value("database/path", defaultPath).toString();
}

void Config::setDatabasePath(const QString& path) {
    m_settings->setValue("database/path", path);
}

QString Config::getLastUsedPort() const {
    return m_settings->value("hardware/last_port", "COM1").toString();
}

void Config::setLastUsedPort(const QString& port) {
    m_settings->setValue("hardware/last_port", port);
}

double Config::getDefaultSpeed() const {
    return m_settings->value("hardware/default_speed", Constants::DEFAULT_SPEED_MM_PER_MIN).toDouble();
}

void Config::setDefaultSpeed(double speed) {
    m_settings->setValue("hardware/default_speed", speed);
}

bool Config::isDarkTheme() const {
    return m_settings->value("ui/dark_theme", true).toBool();
}

void Config::setDarkTheme(bool enabled) {
    m_settings->setValue("ui/dark_theme", enabled);
}

QString Config::getDefaultTestMethod() const {
    return m_settings->value("test/default_method", Constants::METHOD_ISO_527_2).toString();
}

void Config::setDefaultTestMethod(const QString& method) {
    m_settings->setValue("test/default_method", method);
}

double Config::getDefaultGaugeLength() const {
    return m_settings->value("test/default_gauge_length", 50.0).toDouble();
}

void Config::setDefaultGaugeLength(double length) {
    m_settings->setValue("test/default_gauge_length", length);
}

QString Config::getExportPath() const {
    QString defaultPath = QStandardPaths::writableLocation(QStandardPaths::DocumentsLocation) + "/HorizonUTM/Exports";
    return m_settings->value("export/path", defaultPath).toString();
}

void Config::setExportPath(const QString& path) {
    m_settings->setValue("export/path", path);
}

QString Config::getAppDataPath() const {
    QString path = QStandardPaths::writableLocation(QStandardPaths::AppDataLocation);
    QDir().mkpath(path);
    return path;
}

} // namespace HorizonUTM
