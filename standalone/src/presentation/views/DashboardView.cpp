#include "DashboardView.h"
#include "TestConfigDialog.h"
#include "../widgets/RealtimeChartWidget.h"
#include "../widgets/MetricWidget.h"
#include "application/controllers/TestController.h"
#include "application/controllers/HardwareController.h"
#include "domain/value_objects/SensorData.h"

#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGroupBox>
#include <QLabel>
#include <QMessageBox>
#include <QDateTime>

namespace HorizonUTM {

DashboardView::DashboardView(TestController* testController,
                             HardwareController* hardwareController,
                             QWidget* parent)
    : QWidget(parent)
    , m_testController(testController)
    , m_hardwareController(hardwareController)
    , m_chartWidget(nullptr)
    , m_forceWidget(nullptr)
    , m_stressWidget(nullptr)
    , m_strainWidget(nullptr)
    , m_timeWidget(nullptr)
{
    setupUI();
    setupConnections();
}

void DashboardView::setupUI() {
    QVBoxLayout* mainLayout = new QVBoxLayout(this);
    mainLayout->setContentsMargins(10, 10, 10, 10);
    mainLayout->setSpacing(10);

    // Chart section
    QGroupBox* chartGroup = new QGroupBox("Stress-Strain Curve", this);
    QVBoxLayout* chartLayout = new QVBoxLayout(chartGroup);

    m_chartWidget = new RealtimeChartWidget(this);
    chartLayout->addWidget(m_chartWidget);

    mainLayout->addWidget(chartGroup, 3);  // 75% of space

    // Metrics section
    QGroupBox* metricsGroup = new QGroupBox("Current Values", this);
    QHBoxLayout* metricsLayout = new QHBoxLayout(metricsGroup);
    metricsLayout->setSpacing(15);

    m_forceWidget = new MetricWidget("Force", "N", this);
    m_forceWidget->setDecimals(1);
    metricsLayout->addWidget(m_forceWidget);

    m_stressWidget = new MetricWidget("Stress", "MPa", this);
    m_stressWidget->setDecimals(2);
    metricsLayout->addWidget(m_stressWidget);

    m_strainWidget = new MetricWidget("Strain", "%", this);
    m_strainWidget->setDecimals(3);
    metricsLayout->addWidget(m_strainWidget);

    m_timeWidget = new MetricWidget("Time", "s", this);
    m_timeWidget->setDecimals(0);
    metricsLayout->addWidget(m_timeWidget);

    mainLayout->addWidget(metricsGroup, 1);  // 25% of space
}

void DashboardView::setupConnections() {
    // Connect to hardware controller signals
    connect(m_hardwareController, &HardwareController::sensorDataReceived,
            this, &DashboardView::onSensorDataReceived);

    connect(m_hardwareController, &HardwareController::testStarted,
            this, &DashboardView::onTestStarted);

    connect(m_hardwareController, &HardwareController::testCompleted,
            this, &DashboardView::onTestCompleted);
}

void DashboardView::startTest() {
    // Check if hardware is connected
    if (!m_hardwareController->isConnected()) {
        QMessageBox::warning(this, "Hardware Not Connected",
                           "Please connect to hardware before starting a test.");
        return;
    }

    // Show configuration dialog
    TestConfigDialog dialog(this);

    if (dialog.exec() == QDialog::Accepted) {
        // Get parameters from dialog
        TestParametersDTO params = dialog.getTestParameters();

        // Create test from parameters
        Test test = m_testController->createNewTest();
        test.setSampleName(params.sampleName);
        test.setOperatorName(params.operatorName);
        test.setTestMethod(params.testMethod);
        test.setWidth(params.width);
        test.setThickness(params.thickness);
        test.setGaugeLength(params.gaugeLength);
        test.setSpeed(params.speed);
        test.setForceLimit(params.forceLimit);
        test.setTemperature(params.temperature);
        test.setNotes(params.notes);

        // Start test
        if (!m_hardwareController->startTest(test)) {
            QMessageBox::critical(this, "Test Start Failed",
                                "Failed to start test. Check logs for details.");
        }
    }
}

void DashboardView::onSensorDataReceived(const SensorData& data) {
    // Update chart
    m_chartWidget->onSensorDataReceived(data);

    // Update metrics
    m_forceWidget->setValue(data.force);
    m_stressWidget->setValue(data.stress);
    m_strainWidget->setValue(data.strain);

    // Update time
    if (!m_testStartTime.isNull()) {
        qint64 elapsedMs = m_testStartTime.msecsTo(QDateTime::currentDateTime());
        double elapsedSeconds = elapsedMs / 1000.0;
        m_timeWidget->setValue(elapsedSeconds);
    }
}

void DashboardView::onTestStarted(int /*testId*/) {
    // Clear previous data
    m_chartWidget->clearData();

    // Reset metrics
    m_forceWidget->setValue(0.0);
    m_stressWidget->setValue(0.0);
    m_strainWidget->setValue(0.0);
    m_timeWidget->setValue(0.0);

    // Start timer
    m_testStartTime = QDateTime::currentDateTime();
}

void DashboardView::onTestCompleted(int /*testId*/) {
    // Stop timer
    m_testStartTime = QDateTime();
}

} // namespace HorizonUTM
