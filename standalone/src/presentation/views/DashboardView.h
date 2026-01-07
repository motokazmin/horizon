#pragma once

#include <QWidget>
#include <QDateTime>

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
    
    QDateTime m_testStartTime;
};

} // namespace HorizonUTM
