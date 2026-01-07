#pragma once

#include <QObject>
#include <QString>
#include <QVector>
#include <memory>
#include "domain/interfaces/IExportService.h"
#include "domain/entities/Test.h"

namespace HorizonUTM {

/**
 * @brief Controller for data export operations
 */
class DataExportController : public QObject {
    Q_OBJECT

public:
    explicit DataExportController(QObject* parent = nullptr);
    ~DataExportController() override = default;
    
    /**
     * @brief Register export service
     * @param service Export service (CSV, PDF, etc.)
     */
    void registerExportService(IExportService* service);
    
    /**
     * @brief Get available export formats
     * @return List of format extensions
     */
    QStringList getAvailableFormats() const;
    
    /**
     * @brief Export single test
     * @param test Test to export
     * @param filePath Output file path
     * @param format Export format (e.g., "csv")
     * @return true if successful
     */
    bool exportTest(const Test& test, const QString& filePath, const QString& format = "csv");
    
    /**
     * @brief Export multiple tests
     * @param tests Tests to export
     * @param filePath Output file path
     * @param format Export format
     * @return true if successful
     */
    bool exportTests(const QVector<Test>& tests, const QString& filePath, const QString& format = "csv");
    
    /**
     * @brief Get file type description for format
     */
    QString getFileTypeDescription(const QString& format) const;

signals:
    /**
     * @brief Emitted when export starts
     */
    void exportStarted();
    
    /**
     * @brief Emitted when export completes
     */
    void exportCompleted(const QString& filePath);
    
    /**
     * @brief Emitted on export error
     */
    void exportFailed(const QString& error);

private:
    /**
     * @brief Find export service by format
     */
    IExportService* findService(const QString& format) const;

private:
    QVector<IExportService*> m_exportServices;
};

} // namespace HorizonUTM
