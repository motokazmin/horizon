#pragma once

#include <QWidget>
#include <QDateTime>
#include <QProgressBar>
#include <QLabel>

namespace HorizonUTM {

class TestController;
class HardwareController;
class RealtimeChartWidget;
class MetricWidget;
struct SensorData;

/**
 * @brief Main dashboard view
 */
class DashboardView : public QWidget {
    Q_OBJECT

public:
    explicit DashboardView(TestController* testController,
                          HardwareController* hardwareController,
                          QWidget* parent = nullptr);

    void startTest();

private slots:
    void onSensorDataReceived(const SensorData& data);
    void onTestStarted(int testId);
    void onTestCompleted(int testId);

private:
    void setupUI();
    void setupConnections();

private:
    TestController* m_testController;
    HardwareController* m_hardwareController;

    // UI Components
    RealtimeChartWidget* m_chartWidget;
    MetricWidget* m_forceWidget;
    MetricWidget* m_stressWidget;
    MetricWidget* m_strainWidget;
    MetricWidget* m_timeWidget;

    QProgressBar* m_progressBar;
    QLabel* m_progressLabel;

    QDateTime m_testStartTime;
    double m_expectedDuration;  // Expected test duration in seconds
};

} // namespace HorizonUTM
