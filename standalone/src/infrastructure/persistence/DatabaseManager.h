#pragma once

#include <QSqlDatabase>
#include <QString>
#include <QSqlError>

namespace HorizonUTM {

/**
 * @brief Database manager for SQLite operations
 * 
 * Handles database connection, initialization, and schema management
 */
class DatabaseManager {
public:
    /**
     * @brief Get singleton instance
     */
    static DatabaseManager& instance();
    
    /**
     * @brief Initialize database connection
     * @param dbPath Path to SQLite database file
     * @return true if successful
     */
    bool initialize(const QString& dbPath);
    
    /**
     * @brief Close database connection
     */
    void close();
    
    /**
     * @brief Check if database is open
     */
    bool isOpen() const;
    
    /**
     * @brief Get database instance
     */
    QSqlDatabase database() const;
    
    /**
     * @brief Get last error
     */
    QString lastError() const;
    
    /**
     * @brief Create database schema
     * @return true if successful
     */
    bool createSchema();
    
    /**
     * @brief Check if schema exists
     */
    bool schemaExists() const;
    
    /**
     * @brief Execute SQL file
     * @param filePath Path to SQL file
     * @return true if successful
     */
    bool executeSqlFile(const QString& filePath);

private:
    DatabaseManager();
    ~DatabaseManager();
    
    // Disable copy
    DatabaseManager(const DatabaseManager&) = delete;
    DatabaseManager& operator=(const DatabaseManager&) = delete;
    
    /**
     * @brief Create tables
     */
    bool createTables();
    
    /**
     * @brief Create indexes
     */
    bool createIndexes();
    
    /**
     * @brief Create triggers
     */
    bool createTriggers();

private:
    QSqlDatabase m_db;
    QString m_lastError;
    bool m_initialized;
};

} // namespace HorizonUTM
