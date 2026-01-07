#pragma once

#include <QWidget>

namespace HorizonUTM {

class TestController;
class HardwareController;

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

private:
    TestController* m_testController;
    HardwareController* m_hardwareController;
};

} // namespace HorizonUTM
