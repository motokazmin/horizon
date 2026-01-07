#pragma once

#include <QMainWindow>
#include <QStackedWidget>
#include <QToolBar>
#include <QStatusBar>
#include <QLabel>
#include <memory>

namespace HorizonUTM {

class DashboardView;
class SampleQueueView;
class ResultsView;
class SettingsDialog;
class TestController;
class HardwareController;
class DataExportController;
class StatusIndicator;

/**
 * @brief Main application window
 */
class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    explicit MainWindow(TestController* testController,
                       HardwareController* hardwareController,
                       DataExportController* exportController,
                       QWidget* parent = nullptr);
    ~MainWindow() override;

protected:
    void closeEvent(QCloseEvent* event) override;

private slots:
    // Navigation
    void showDashboard();
    void showSampleQueue();
    void showResults();
    void showSettings();
    
    // Actions
    void onConnectHardware();
    void onDisconnectHardware();
    void onStartTest();
    void onStopTest();
    void onPauseTest();
    void onZeroSensors();
    void onExportData();
    
    // Status updates
    void onHardwareConnected();
    void onHardwareDisconnected();
    void onTestStarted();
    void onTestCompleted();
    void onMachineStateChanged();

private:
    void setupUi();
    void setupMenuBar();
    void setupToolBar();
    void setupStatusBar();
    void setupConnections();
    void updateActions();

private:
    // Controllers
    TestController* m_testController;
    HardwareController* m_hardwareController;
    DataExportController* m_exportController;
    
    // Views
    QStackedWidget* m_stackedWidget;
    DashboardView* m_dashboardView;
    SampleQueueView* m_sampleQueueView;
    ResultsView* m_resultsView;
    SettingsDialog* m_settingsDialog;
    
    // UI Elements
    QToolBar* m_toolBar;
    StatusIndicator* m_statusIndicator;
    QLabel* m_statusLabel;
    
    // Actions
    QAction* m_connectAction;
    QAction* m_disconnectAction;
    QAction* m_startTestAction;
    QAction* m_stopTestAction;
    QAction* m_pauseTestAction;
    QAction* m_zeroAction;
    QAction* m_exportAction;
    QAction* m_dashboardAction;
    QAction* m_sampleQueueAction;
    QAction* m_resultsAction;
    QAction* m_settingsAction;
    QAction* m_exitAction;
};

} // namespace HorizonUTM
