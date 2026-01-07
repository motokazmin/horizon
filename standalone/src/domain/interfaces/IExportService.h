#pragma once

#include <QString>
#include <QStringList>
#include <QVector>
#include "domain/entities/Test.h"

namespace HorizonUTM {

/**
 * @brief Interface for exporting test data
 * 
 * This interface allows different export formats (CSV, PDF, Excel)
 * to be implemented and used interchangeably
 */
class IExportService {
public:
    virtual ~IExportService() = default;
    
    /**
     * @brief Export test data to a file
     * @param test Test entity to export
     * @param filePath Full path where file should be saved
     * @return true if export successful
     */
    virtual bool exportTest(const Test& test, const QString& filePath) = 0;
    
    /**
     * @brief Export multiple tests to a file
     * @param tests Vector of tests to export
     * @param filePath Full path where file should be saved
     * @return true if export successful
     */
    virtual bool exportTests(const QVector<Test>& tests, const QString& filePath) = 0;
    
    /**
     * @brief Get supported file extensions
     * @return List of extensions (e.g., ["csv"])
     */
    virtual QStringList getSupportedExtensions() const = 0;
    
    /**
     * @brief Get file type description
     * @return Description for file dialog (e.g., "CSV Files (*.csv)")
     */
    virtual QString getFileTypeDescription() const = 0;
};

} // namespace HorizonUTM
