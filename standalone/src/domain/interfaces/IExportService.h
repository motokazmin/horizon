#pragma once

#include <QString>
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
     * @brief Get the file extension for this export format
     * @return File extension (e.g., "csv", "pdf", "xlsx")
     */
    virtual QString getFileExtension() const = 0;
    
    /**
     * @brief Get file dialog filter string
     * @return Filter string for QFileDialog (e.g., "CSV Files (*.csv)")
     */
    virtual QString getFilterString() const = 0;
};

} // namespace HorizonUTM
