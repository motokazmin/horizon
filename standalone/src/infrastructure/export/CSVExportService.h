#pragma once

#include "domain/interfaces/IExportService.h"
#include "domain/entities/Test.h"
#include <QString>

namespace HorizonUTM {

/**
 * @brief CSV export service implementation
 * 
 * Exports test data and results to CSV format
 */
class CSVExportService : public IExportService {
public:
    CSVExportService() = default;
    ~CSVExportService() override = default;
    
    /**
     * @brief Export test to CSV file
     * @param test Test to export
     * @param filePath Output file path
     * @return true if successful
     */
    bool exportTest(const Test& test, const QString& filePath) override;
    
    /**
     * @brief Export multiple tests to CSV
     * @param tests Tests to export
     * @param filePath Output file path
     * @return true if successful
     */
    bool exportTests(const QVector<Test>& tests, const QString& filePath) override;
    
    /**
     * @brief Get supported file extensions
     * @return List of extensions (e.g., ["csv"])
     */
    QStringList getSupportedExtensions() const override;
    
    /**
     * @brief Get file type description
     */
    QString getFileTypeDescription() const override;

private:
    /**
     * @brief Write CSV header
     */
    QString createCsvHeader() const;
    
    /**
     * @brief Convert test to CSV row
     */
    QString testToCsvRow(const Test& test) const;
    
    /**
     * @brief Escape CSV field
     */
    QString escapeCsvField(const QString& field) const;
};

} // namespace HorizonUTM
