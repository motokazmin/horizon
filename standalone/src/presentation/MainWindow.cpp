#include "MainWindow.h"
#include "views/DashboardView.h"
#include "views/SampleQueueView.h"
#include "views/SettingsDialog.h"
#include "widgets/StatusIndicator.h"
#include "application/controllers/TestController.h"
#include "application/controllers/HardwareController.h"
#include "application/controllers/DataExportController.h"
#include "core/Logger.h"

#include <QMenuBar>
#include <QToolBar>
#include <QStatusBar>
#include <QMessageBox>
#include <QFileDialog>
#include <QCloseEvent>

namespace HorizonUTM {

MainWindow::MainWindow(TestController* testController,
                       HardwareController* hardwareController,
                       DataExportController* exportController,
                       QWidget* parent)
    : QMainWindow(parent)
    , m_testController(testController)
    , m_hardwareController(hardwareController)
    , m_exportController(exportController)
    , m_stackedWidget(nullptr)
    , m_dashboardView(nullptr)
    , m_sampleQueueView(nullptr)
    , m_settingsDialog(nullptr)
    , m_toolBar(nullptr)
    , m_statusIndicator(nullptr)
    , m_statusLabel(nullptr)
{
    setupUi();
    setupMenuBar();
    setupToolBar();
    setupStatusBar();
    setupConnections();
    updateActions();
    
    setWindowTitle("Horizon UTM - Universal Testing Machine");
    resize(1280, 800);
    
    LOG_INFO("MainWindow created");
}

MainWindow::~MainWindow() {
    LOG_INFO("MainWindow destroyed");
}

void MainWindow::setupUi() {
    // Create central widget
    m_stackedWidget = new QStackedWidget(this);
    setCentralWidget(m_stackedWidget);
    
    // Create views
    m_dashboardView = new DashboardView(m_testController, m_hardwareController, this);
    m_sampleQueueView = new SampleQueueView(m_testController, this);
    
    // Add views to stack
    m_stackedWidget->addWidget(m_dashboardView);
    m_stackedWidget->addWidget(m_sampleQueueView);
    
    // Show dashboard by default
    m_stackedWidget->setCurrentWidget(m_dashboardView);
    
    // Create settings dialog
    m_settingsDialog = new SettingsDialog(this);
}

void MainWindow::setupMenuBar() {
    QMenuBar* menuBar = new QMenuBar(this);
    setMenuBar(menuBar);
    
    // File menu
    QMenu* fileMenu = menuBar->addMenu("&File");
    
    m_exportAction = fileMenu->addAction("&Export Data...");
    m_exportAction->setShortcut(QKeySequence("Ctrl+E"));
    connect(m_exportAction, &QAction::triggered, this, &MainWindow::onExportData);
    
    fileMenu->addSeparator();
    
    m_exitAction = fileMenu->addAction("E&xit");
    m_exitAction->setShortcut(QKeySequence("Ctrl+Q"));
    connect(m_exitAction, &QAction::triggered, this, &MainWindow::close);
    
    // Hardware menu
    QMenu* hardwareMenu = menuBar->addMenu("&Hardware");
    
    m_connectAction = hardwareMenu->addAction("&Connect");
    connect(m_connectAction, &QAction::triggered, this, &MainWindow::onConnectHardware);
    
    m_disconnectAction = hardwareMenu->addAction("&Disconnect");
    connect(m_disconnectAction, &QAction::triggered, this, &MainWindow::onDisconnectHardware);
    
    hardwareMenu->addSeparator();
    
    m_zeroAction = hardwareMenu->addAction("&Zero Sensors");
    m_zeroAction->setShortcut(QKeySequence("Ctrl+Z"));
    connect(m_zeroAction, &QAction::triggered, this, &MainWindow::onZeroSensors);
    
    // Test menu
    QMenu* testMenu = menuBar->addMenu("&Test");
    
    m_startTestAction = testMenu->addAction("&Start Test");
    m_startTestAction->setShortcut(QKeySequence("F5"));
    connect(m_startTestAction, &QAction::triggered, this, &MainWindow::onStartTest);
    
    m_pauseTestAction = testMenu->addAction("&Pause Test");
    m_pauseTestAction->setShortcut(QKeySequence("F6"));
    connect(m_pauseTestAction, &QAction::triggered, this, &MainWindow::onPauseTest);
    
    m_stopTestAction = testMenu->addAction("S&top Test");
    m_stopTestAction->setShortcut(QKeySequence("F7"));
    connect(m_stopTestAction, &QAction::triggered, this, &MainWindow::onStopTest);
    
    // View menu
    QMenu* viewMenu = menuBar->addMenu("&View");
    
    m_dashboardAction = viewMenu->addAction("&Dashboard");
    m_dashboardAction->setShortcut(QKeySequence("Ctrl+1"));
    connect(m_dashboardAction, &QAction::triggered, this, &MainWindow::showDashboard);
    
    m_sampleQueueAction = viewMenu->addAction("&Sample Queue");
    m_sampleQueueAction->setShortcut(QKeySequence("Ctrl+2"));
    connect(m_sampleQueueAction, &QAction::triggered, this, &MainWindow::showSampleQueue);
    
    // Settings menu
    QMenu* settingsMenu = menuBar->addMenu("&Settings");
    
    m_settingsAction = settingsMenu->addAction("&Preferences...");
    m_settingsAction->setShortcut(QKeySequence("Ctrl+,"));
    connect(m_settingsAction, &QAction::triggered, this, &MainWindow::showSettings);
}

void MainWindow::setupToolBar() {
    m_toolBar = addToolBar("Main Toolbar");
    m_toolBar->setMovable(false);
    
    // Hardware actions
    m_toolBar->addAction(m_connectAction);
    m_toolBar->addAction(m_disconnectAction);
    m_toolBar->addSeparator();
    
    // Test actions
    m_toolBar->addAction(m_startTestAction);
    m_toolBar->addAction(m_pauseTestAction);
    m_toolBar->addAction(m_stopTestAction);
    m_toolBar->addSeparator();
    
    // Utility actions
    m_toolBar->addAction(m_zeroAction);
    m_toolBar->addAction(m_exportAction);
}

void MainWindow::setupStatusBar() {
    QStatusBar* statusBar = new QStatusBar(this);
    setStatusBar(statusBar);
    
    // Status indicator
    m_statusIndicator = new StatusIndicator(this);
    statusBar->addPermanentWidget(m_statusIndicator);
    
    // Status label
    m_statusLabel = new QLabel("Ready", this);
    statusBar->addWidget(m_statusLabel);
}

void MainWindow::setupConnections() {
    // Hardware controller signals
    connect(m_hardwareController, &HardwareController::hardwareConnected,
            this, &MainWindow::onHardwareConnected);
    connect(m_hardwareController, &HardwareController::hardwareDisconnected,
            this, &MainWindow::onHardwareDisconnected);
    connect(m_hardwareController, &HardwareController::testStarted,
            this, &MainWindow::onTestStarted);
    connect(m_hardwareController, &HardwareController::testCompleted,
            this, &MainWindow::onTestCompleted);
    connect(m_hardwareController, &HardwareController::machineStateChanged,
            this, &MainWindow::onMachineStateChanged);
}

void MainWindow::updateActions() {
    bool connected = m_hardwareController->isConnected();
    bool testRunning = m_hardwareController->isTestRunning();
    
    m_connectAction->setEnabled(!connected);
    m_disconnectAction->setEnabled(connected && !testRunning);
    m_startTestAction->setEnabled(connected && !testRunning);
    m_stopTestAction->setEnabled(testRunning);
    m_pauseTestAction->setEnabled(testRunning);
    m_zeroAction->setEnabled(connected && !testRunning);
}

// Navigation slots

void MainWindow::showDashboard() {
    m_stackedWidget->setCurrentWidget(m_dashboardView);
    m_statusLabel->setText("Dashboard");
}

void MainWindow::showSampleQueue() {
    m_stackedWidget->setCurrentWidget(m_sampleQueueView);
    m_statusLabel->setText("Sample Queue");
}

void MainWindow::showSettings() {
    m_settingsDialog->exec();
}

// Action slots

void MainWindow::onConnectHardware() {
    if (m_hardwareController->connectToHardware("mock")) {
        LOG_INFO("Hardware connection initiated");
    } else {
        QMessageBox::critical(this, "Connection Error", "Failed to connect to hardware");
    }
}

void MainWindow::onDisconnectHardware() {
    if (m_hardwareController->disconnectFromHardware()) {
        LOG_INFO("Hardware disconnected");
    }
}

void MainWindow::onStartTest() {
    // This will be handled by DashboardView
    m_dashboardView->startTest();
}

void MainWindow::onStopTest() {
    auto result = QMessageBox::question(this, "Stop Test",
                                        "Are you sure you want to stop the current test?",
                                        QMessageBox::Yes | QMessageBox::No);
    
    if (result == QMessageBox::Yes) {
        m_hardwareController->stopTest();
    }
}

void MainWindow::onPauseTest() {
    m_hardwareController->pauseTest();
}

void MainWindow::onZeroSensors() {
    if (m_hardwareController->zeroSensors()) {
        m_statusLabel->setText("Sensors zeroed");
        LOG_INFO("Sensors zeroed");
    } else {
        QMessageBox::warning(this, "Zero Error", "Failed to zero sensors");
    }
}

void MainWindow::onExportData() {
    QString fileName = QFileDialog::getSaveFileName(this,
                                                    "Export Test Data",
                                                    "",
                                                    "CSV Files (*.csv)");
    
    if (fileName.isEmpty()) {
        return;
    }
    
    // Get all completed tests
    QVector<Test> tests = m_testController->getTestsByStatus(TestStatus::Completed);
    
    if (tests.isEmpty()) {
        QMessageBox::information(this, "Export", "No completed tests to export");
        return;
    }
    
    if (m_exportController->exportTests(tests, fileName, "csv")) {
        QMessageBox::information(this, "Export", "Data exported successfully");
    } else {
        QMessageBox::critical(this, "Export Error", "Failed to export data");
    }
}

// Status update slots

void MainWindow::onHardwareConnected() {
    m_statusIndicator->setConnected(true);
    m_statusLabel->setText("Hardware connected");
    updateActions();
    LOG_INFO("Hardware connected - UI updated");
}

void MainWindow::onHardwareDisconnected() {
    m_statusIndicator->setConnected(false);
    m_statusLabel->setText("Hardware disconnected");
    updateActions();
    LOG_INFO("Hardware disconnected - UI updated");
}

void MainWindow::onTestStarted() {
    m_statusLabel->setText("Test running...");
    updateActions();
}

void MainWindow::onTestCompleted() {
    m_statusLabel->setText("Test completed");
    updateActions();
    QMessageBox::information(this, "Test Complete", "Test completed successfully");
}

void MainWindow::onMachineStateChanged() {
    updateActions();
}

void MainWindow::closeEvent(QCloseEvent* event) {
    if (m_hardwareController->isTestRunning()) {
        auto result = QMessageBox::question(this, "Exit",
                                           "A test is currently running. Are you sure you want to exit?",
                                           QMessageBox::Yes | QMessageBox::No);
        
        if (result == QMessageBox::No) {
            event->ignore();
            return;
        }
        
        m_hardwareController->stopTest();
    }
    
    LOG_INFO("Application closing");
    event->accept();
}

} // namespace HorizonUTM
