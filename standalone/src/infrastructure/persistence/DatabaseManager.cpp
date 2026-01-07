#include "DatabaseManager.h"
#include "core/Logger.h"
#include <QSqlQuery>
#include <QSqlError>
#include <QFile>
#include <QTextStream>
#include <QDir>

namespace HorizonUTM {

DatabaseManager& DatabaseManager::instance() {
    static DatabaseManager instance;
    return instance;
}

DatabaseManager::DatabaseManager()
    : m_initialized(false)
{
}

DatabaseManager::~DatabaseManager() {
    close();
}

bool DatabaseManager::initialize(const QString& dbPath) {
    if (m_initialized) {
        LOG_WARNING("Database already initialized");
        return true;
    }
    
    LOG_INFO(QString("Initializing database: %1").arg(dbPath));
    
    // Create directory if it doesn't exist
    QFileInfo fileInfo(dbPath);
    QDir dir = fileInfo.absoluteDir();
    if (!dir.exists()) {
        if (!dir.mkpath(".")) {
            m_lastError = QString("Failed to create directory: %1").arg(dir.absolutePath());
            LOG_ERROR(m_lastError);
            return false;
        }
    }
    
    // Create database connection
    m_db = QSqlDatabase::addDatabase("QSQLITE");
    m_db.setDatabaseName(dbPath);
    
    if (!m_db.open()) {
        m_lastError = m_db.lastError().text();
        LOG_ERROR(QString("Failed to open database: %1").arg(m_lastError));
        return false;
    }
    
    LOG_INFO("Database opened successfully");
    
    // Check if schema exists
    if (!schemaExists()) {
        LOG_INFO("Schema not found, creating...");
        if (!createSchema()) {
            m_lastError = "Failed to create schema";
            LOG_ERROR(m_lastError);
            return false;
        }
    }
    
    m_initialized = true;
    LOG_INFO("Database initialized successfully");
    
    return true;
}

void DatabaseManager::close() {
    if (m_db.isOpen()) {
        m_db.close();
        LOG_INFO("Database closed");
    }
    m_initialized = false;
}

bool DatabaseManager::isOpen() const {
    return m_db.isOpen();
}

QSqlDatabase DatabaseManager::database() const {
    return m_db;
}

QString DatabaseManager::lastError() const {
    return m_lastError;
}

bool DatabaseManager::createSchema() {
    if (!createTables()) {
        return false;
    }
    
    if (!createIndexes()) {
        return false;
    }
    
    if (!createTriggers()) {
        return false;
    }
    
    LOG_INFO("Database schema created successfully");
    return true;
}

bool DatabaseManager::schemaExists() const {
    QSqlQuery query(m_db);
    query.prepare("SELECT name FROM sqlite_master WHERE type='table' AND name='tests'");
    
    if (!query.exec()) {
        return false;
    }
    
    return query.next();
}

bool DatabaseManager::executeSqlFile(const QString& filePath) {
    QFile file(filePath);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        m_lastError = QString("Cannot open SQL file: %1").arg(filePath);
        LOG_ERROR(m_lastError);
        return false;
    }
    
    QTextStream in(&file);
    QString sql = in.readAll();
    file.close();
    
    // Split by semicolons and execute each statement
    QStringList statements = sql.split(';', Qt::SkipEmptyParts);
    
    QSqlQuery query(m_db);
    for (const QString& statement : statements) {
        QString trimmed = statement.trimmed();
        if (trimmed.isEmpty() || trimmed.startsWith("--")) {
            continue;
        }
        
        if (!query.exec(trimmed)) {
            m_lastError = query.lastError().text();
            LOG_ERROR(QString("SQL error: %1").arg(m_lastError));
            return false;
        }
    }
    
    return true;
}

bool DatabaseManager::createTables() {
    QSqlQuery query(m_db);
    
    // Tests table
    QString testsTable = R"(
        CREATE TABLE IF NOT EXISTS tests (
            id INTEGER PRIMARY KEY AUTOINCREMENT,
            sample_name TEXT NOT NULL,
            operator_name TEXT NOT NULL,
            test_method TEXT NOT NULL,
            width REAL NOT NULL,
            thickness REAL NOT NULL,
            gauge_length REAL NOT NULL,
            speed REAL NOT NULL,
            force_limit REAL NOT NULL,
            temperature REAL,
            status TEXT NOT NULL,
            start_time DATETIME,
            end_time DATETIME,
            max_stress REAL,
            yield_stress REAL,
            ultimate_stress REAL,
            break_stress REAL,
            elastic_modulus REAL,
            elongation_at_break REAL,
            notes TEXT,
            created_at DATETIME DEFAULT CURRENT_TIMESTAMP,
            updated_at DATETIME DEFAULT CURRENT_TIMESTAMP
        )
    )";
    
    if (!query.exec(testsTable)) {
        m_lastError = query.lastError().text();
        LOG_ERROR(QString("Failed to create tests table: %1").arg(m_lastError));
        return false;
    }
    
    // Test data points table
    QString dataPointsTable = R"(
        CREATE TABLE IF NOT EXISTS test_data_points (
            id INTEGER PRIMARY KEY AUTOINCREMENT,
            test_id INTEGER NOT NULL,
            timestamp INTEGER NOT NULL,
            time_seconds REAL NOT NULL,
            force REAL NOT NULL,
            extension REAL NOT NULL,
            stress REAL NOT NULL,
            strain REAL NOT NULL,
            temperature REAL,
            FOREIGN KEY (test_id) REFERENCES tests(id) ON DELETE CASCADE
        )
    )";
    
    if (!query.exec(dataPointsTable)) {
        m_lastError = query.lastError().text();
        LOG_ERROR(QString("Failed to create data points table: %1").arg(m_lastError));
        return false;
    }
    
    // Samples queue table
    QString samplesTable = R"(
        CREATE TABLE IF NOT EXISTS samples (
            id INTEGER PRIMARY KEY AUTOINCREMENT,
            name TEXT NOT NULL,
            width REAL NOT NULL,
            thickness REAL NOT NULL,
            gauge_length REAL NOT NULL,
            test_method TEXT NOT NULL,
            operator_name TEXT,
            status TEXT NOT NULL,
            notes TEXT,
            created_at DATETIME DEFAULT CURRENT_TIMESTAMP
        )
    )";
    
    if (!query.exec(samplesTable)) {
        m_lastError = query.lastError().text();
        LOG_ERROR(QString("Failed to create samples table: %1").arg(m_lastError));
        return false;
    }
    
    LOG_INFO("Database tables created");
    return true;
}

bool DatabaseManager::createIndexes() {
    QSqlQuery query(m_db);
    
    QStringList indexes = {
        "CREATE INDEX IF NOT EXISTS idx_tests_status ON tests(status)",
        "CREATE INDEX IF NOT EXISTS idx_tests_start_time ON tests(start_time)",
        "CREATE INDEX IF NOT EXISTS idx_data_points_test_id ON test_data_points(test_id)",
        "CREATE INDEX IF NOT EXISTS idx_data_points_timestamp ON test_data_points(timestamp)",
        "CREATE INDEX IF NOT EXISTS idx_samples_status ON samples(status)"
    };
    
    for (const QString& indexSql : indexes) {
        if (!query.exec(indexSql)) {
            m_lastError = query.lastError().text();
            LOG_ERROR(QString("Failed to create index: %1").arg(m_lastError));
            return false;
        }
    }
    
    LOG_INFO("Database indexes created");
    return true;
}

bool DatabaseManager::createTriggers() {
    QSqlQuery query(m_db);
    
    // Update timestamp trigger
    QString trigger = R"(
        CREATE TRIGGER IF NOT EXISTS update_tests_timestamp 
        AFTER UPDATE ON tests
        FOR EACH ROW
        BEGIN
            UPDATE tests SET updated_at = CURRENT_TIMESTAMP WHERE id = NEW.id;
        END
    )";
    
    if (!query.exec(trigger)) {
        m_lastError = query.lastError().text();
        LOG_ERROR(QString("Failed to create trigger: %1").arg(m_lastError));
        return false;
    }
    
    LOG_INFO("Database triggers created");
    return true;
}

} // namespace HorizonUTM
