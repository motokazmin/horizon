#include "DataExportController.h"
#include "core/Logger.h"

namespace HorizonUTM {

DataExportController::DataExportController(QObject* parent)
    : QObject(parent)
{
    LOG_INFO("DataExportController created");
}

void DataExportController::registerExportService(IExportService* service) {
    if (!service) {
        LOG_ERROR("Cannot register null export service");
        return;
    }
    
    m_exportServices.append(service);
    
    QStringList extensions = service->getSupportedExtensions();
    LOG_INFO(QString("Registered export service: %1").arg(extensions.join(", ")));
}

QStringList DataExportController::getAvailableFormats() const {
    QStringList formats;
    
    for (IExportService* service : m_exportServices) {
        formats.append(service->getSupportedExtensions());
    }
    
    return formats;
}

bool DataExportController::exportTest(const Test& test, const QString& filePath, const QString& format) {
    IExportService* service = findService(format);
    
    if (!service) {
        QString error = QString("No export service found for format: %1").arg(format);
        LOG_ERROR(error);
        emit exportFailed(error);
        return false;
    }
    
    if (!test.isValid()) {
        QString error = "Cannot export invalid test";
        LOG_ERROR(error);
        emit exportFailed(error);
        return false;
    }
    
    LOG_INFO(QString("Exporting test ID=%1 to %2").arg(test.getId()).arg(filePath));
    emit exportStarted();
    
    bool success = service->exportTest(test, filePath);
    
    if (success) {
        LOG_INFO(QString("Export completed: %1").arg(filePath));
        emit exportCompleted(filePath);
    } else {
        QString error = QString("Export failed: %1").arg(filePath);
        LOG_ERROR(error);
        emit exportFailed(error);
    }
    
    return success;
}

bool DataExportController::exportTests(const QVector<Test>& tests, const QString& filePath, const QString& format) {
    IExportService* service = findService(format);
    
    if (!service) {
        QString error = QString("No export service found for format: %1").arg(format);
        LOG_ERROR(error);
        emit exportFailed(error);
        return false;
    }
    
    if (tests.isEmpty()) {
        QString error = "No tests to export";
        LOG_WARNING(error);
        emit exportFailed(error);
        return false;
    }
    
    LOG_INFO(QString("Exporting %1 tests to %2").arg(tests.size()).arg(filePath));
    emit exportStarted();
    
    bool success = service->exportTests(tests, filePath);
    
    if (success) {
        LOG_INFO(QString("Export completed: %1 tests → %2").arg(tests.size()).arg(filePath));
        emit exportCompleted(filePath);
    } else {
        QString error = QString("Export failed: %1").arg(filePath);
        LOG_ERROR(error);
        emit exportFailed(error);
    }
    
    return success;
}

QString DataExportController::getFileTypeDescription(const QString& format) const {
    IExportService* service = findService(format);
    
    if (service) {
        return service->getFileTypeDescription();
    }
    
    return QString("Unknown Format (*.%1)").arg(format);
}

IExportService* DataExportController::findService(const QString& format) const {
    QString lowerFormat = format.toLower();
    
    for (IExportService* service : m_exportServices) {
        QStringList extensions = service->getSupportedExtensions();
        if (extensions.contains(lowerFormat)) {
            return service;
        }
    }
    
    return nullptr;
}

} // namespace HorizonUTM
