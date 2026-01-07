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
    , m_progressBar(nullptr)
    , m_progressLabel(nullptr)
    , m_expectedDuration(100.0)  // Default: 100 seconds for mock
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

    // Progress section
    QWidget* progressWidget = new QWidget(this);
    QVBoxLayout* progressLayout = new QVBoxLayout(progressWidget);
    progressLayout->setContentsMargins(0, 5, 0, 0);
    progressLayout->setSpacing(5);

    m_progressLabel = new QLabel("Ready", this);
    m_progressLabel->setAlignment(Qt::AlignCenter);
    QFont labelFont = m_progressLabel->font();
    labelFont.setPointSize(10);
    m_progressLabel->setFont(labelFont);
    progressLayout->addWidget(m_progressLabel);

    m_progressBar = new QProgressBar(this);
    m_progressBar->setRange(0, 100);
    m_progressBar->setValue(0);
    m_progressBar->setTextVisible(true);
    m_progressBar->setFormat("%p%");
    m_progressBar->setMaximumHeight(25);
    progressLayout->addWidget(m_progressBar);

    mainLayout->addWidget(progressWidget);
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

    // Update time and progress
    if (!m_testStartTime.isNull()) {
        qint64 elapsedMs = m_testStartTime.msecsTo(QDateTime::currentDateTime());
        double elapsedSeconds = elapsedMs / 1000.0;
        m_timeWidget->setValue(elapsedSeconds);

        // Update progress bar
        double progress = (elapsedSeconds / m_expectedDuration) * 100.0;
        if (progress > 100.0) progress = 100.0;
        m_progressBar->setValue(static_cast<int>(progress));

        // Determine stage based on strain
        QString stage;
        QString color;
        if (data.strain < 0.5) {
            stage = "Elastic";
            color = "#3b82f6";  // Blue
        } else if (data.strain < 1.5) {
            stage = "Yielding";
            color = "#f59e0b";  // Amber
        } else if (data.strain < 6.0) {
            stage = "Plastic Deformation";
            color = "#8b5cf6";  // Purple
        } else {
            stage = "Necking";
            color = "#ef4444";  // Red
        }

        m_progressBar->setStyleSheet(QString("QProgressBar::chunk { background-color: %1; }").arg(color));

        int remaining = static_cast<int>(m_expectedDuration - elapsedSeconds);
        if (remaining < 0) remaining = 0;
        m_progressLabel->setText(QString("Stage: %1 | ~%2s remaining")
                                .arg(stage)
                                .arg(remaining));
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

    // Reset progress
    m_progressBar->setValue(0);
    m_progressBar->setStyleSheet("QProgressBar::chunk { background-color: #3b82f6; }");
    m_progressLabel->setText("Stage: Elastic | Starting test...");

    // Start timer
    m_testStartTime = QDateTime::currentDateTime();
}

void DashboardView::onTestCompleted(int /*testId*/) {
    // Complete progress
    m_progressBar->setValue(100);
    m_progressBar->setStyleSheet("QProgressBar::chunk { background-color: #22c55e; }"); // Green
    m_progressLabel->setText("Test completed!");

    // Stop timer
    m_testStartTime = QDateTime();
}

} // namespace HorizonUTM
