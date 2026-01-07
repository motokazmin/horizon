#include "CSVExportService.h"
#include "core/Logger.h"
#include <QFile>
#include <QTextStream>
#include <QFileInfo>

namespace HorizonUTM {

bool CSVExportService::exportTest(const Test& test, const QString& filePath) {
    QFile file(filePath);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        LOG_ERROR(QString("Failed to open file for writing: %1").arg(filePath));
        return false;
    }
    
    QTextStream out(&file);
    
    // Write header
    out << createCsvHeader() << "\n";
    
    // Write test data
    out << testToCsvRow(test) << "\n";
    
    file.close();
    
    LOG_INFO(QString("Test exported to CSV: %1").arg(filePath));
    return true;
}

bool CSVExportService::exportTests(const QVector<Test>& tests, const QString& filePath) {
    if (tests.isEmpty()) {
        LOG_WARNING("No tests to export");
        return false;
    }
    
    QFile file(filePath);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        LOG_ERROR(QString("Failed to open file for writing: %1").arg(filePath));
        return false;
    }
    
    QTextStream out(&file);
    
    // Write header
    out << createCsvHeader() << "\n";
    
    // Write all tests
    for (const Test& test : tests) {
        out << testToCsvRow(test) << "\n";
    }
    
    file.close();
    
    LOG_INFO(QString("%1 tests exported to CSV: %2").arg(tests.size()).arg(filePath));
    return true;
}

QStringList CSVExportService::getSupportedExtensions() const {
    return QStringList() << "csv";
}

QString CSVExportService::getFileTypeDescription() const {
    return "Comma Separated Values (*.csv)";
}

QString CSVExportService::createCsvHeader() const {
    QStringList headers;
    
    headers << "Test ID"
            << "Sample Name"
            << "Operator"
            << "Test Method"
            << "Status"
            << "Start Time"
            << "End Time"
            << "Duration (s)"
            << "Width (mm)"
            << "Thickness (mm)"
            << "Gauge Length (mm)"
            << "Cross Section (mm²)"
            << "Speed (mm/min)"
            << "Force Limit (N)"
            << "Temperature (°C)"
            << "Max Stress (MPa)"
            << "Yield Stress (MPa)"
            << "Ultimate Stress (MPa)"
            << "Break Stress (MPa)"
            << "Max Strain (%)"
            << "Yield Strain (%)"
            << "Ultimate Strain (%)"
            << "Break Strain (%)"
            << "Elastic Modulus (GPa)"
            << "Elongation at Break (%)"
            << "Data Points"
            << "Notes";
    
    return headers.join(",");
}

QString CSVExportService::testToCsvRow(const Test& test) const {
    QStringList fields;
    
    fields << QString::number(test.getId())
           << escapeCsvField(test.getSampleName())
           << escapeCsvField(test.getOperatorName())
           << escapeCsvField(test.getTestMethod())
           << escapeCsvField(testStatusToString(test.getStatus()))
           << (test.getStartTime().isValid() ? test.getStartTime().toString("yyyy-MM-dd hh:mm:ss") : "")
           << (test.getEndTime().isValid() ? test.getEndTime().toString("yyyy-MM-dd hh:mm:ss") : "")
           << QString::number(test.getDuration())
           << QString::number(test.getWidth(), 'f', 2)
           << QString::number(test.getThickness(), 'f', 2)
           << QString::number(test.getGaugeLength(), 'f', 1)
           << QString::number(test.getCrossSectionArea(), 'f', 2)
           << QString::number(test.getSpeed(), 'f', 2)
           << QString::number(test.getForceLimit(), 'f', 0)
           << QString::number(test.getTemperature(), 'f', 1);
    
    const TestResult& result = test.getResult();
    fields << QString::number(result.maxStress, 'f', 2)
           << QString::number(result.yieldStress, 'f', 2)
           << QString::number(result.ultimateStress, 'f', 2)
           << QString::number(result.breakStress, 'f', 2)
           << QString::number(result.maxStrain, 'f', 3)
           << QString::number(result.yieldStrain, 'f', 3)
           << QString::number(result.ultimateStrain, 'f', 3)
           << QString::number(result.breakStrain, 'f', 3)
           << QString::number(result.elasticModulus, 'f', 3)
           << QString::number(result.elongationAtBreak, 'f', 3)
           << QString::number(test.getDataPointCount())
           << escapeCsvField(test.getNotes());
    
    return fields.join(",");
}

QString CSVExportService::escapeCsvField(const QString& field) const {
    // If field contains comma, quote, or newline, wrap in quotes and escape quotes
    if (field.contains(',') || field.contains('"') || field.contains('\n')) {
        QString escaped = field;
        escaped.replace("\"", "\"\""); // Escape quotes by doubling them
        return "\"" + escaped + "\"";
    }
    return field;
}

} // namespace HorizonUTM
