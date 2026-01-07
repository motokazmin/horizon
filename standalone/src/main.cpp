// Horizon UTM - Main Entry Point
#include <QApplication>
#include "presentation/MainWindow.h"
#include "application/controllers/TestController.h"
#include "application/controllers/HardwareController.h"
#include "application/controllers/DataExportController.h"
#include "infrastructure/hardware/MockUTMDriver.h"
#include "infrastructure/persistence/DatabaseManager.h"
#include "infrastructure/persistence/SQLiteTestRepository.h"
#include "infrastructure/export/CSVExportService.h"
#include "core/Logger.h"
#include "core/Config.h"

using namespace HorizonUTM;

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);
    
    // Setup application metadata
    app.setOrganizationName("HorizonUTM");
    app.setApplicationName("Horizon UTM");
    app.setApplicationVersion("1.0.0 MVP");
    
    // Initialize logger
    Logger::initialize(LogLevel::Debug);
    LOG_INFO("=== Horizon UTM Starting ===");
    
    // Initialize configuration
    Config& config = Config::instance();
    config.load();
    
    // Initialize database
    DatabaseManager& dbManager = DatabaseManager::instance();
    QString dbPath = "horizon_utm.db";
    if (!dbManager.initialize(dbPath)) {
        LOG_ERROR("Failed to initialize database");
        return 1;
    }
    LOG_INFO("Database initialized");
    
    // Create infrastructure components (all through new for proper initialization)
    MockUTMDriver* utmDriver = new MockUTMDriver();
    SQLiteTestRepository* repository = new SQLiteTestRepository();
    CSVExportService* csvExporter = new CSVExportService();
    
    // Create application controllers
    TestController* testController = new TestController(repository);
    HardwareController* hardwareController = new HardwareController(utmDriver, testController);
    DataExportController* exportController = new DataExportController();
    
    // Register export services
    exportController->registerExportService(csvExporter);
    
    // Create and show main window
    MainWindow* mainWindow = new MainWindow(testController, hardwareController, exportController);
    mainWindow->show();
    
    LOG_INFO("Application started successfully");
    
    int result = app.exec();
    
    // Cleanup
    LOG_INFO("Application shutting down");
    
    delete mainWindow;
    delete exportController;
    delete hardwareController;
    delete testController;
    delete csvExporter;
    delete repository;
    delete utmDriver;
    
    dbManager.close();
    
    LOG_INFO("=== Horizon UTM Stopped ===");
    
    return result;
}
