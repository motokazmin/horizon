#include "DashboardView.h"
#include "application/controllers/TestController.h"
#include "application/controllers/HardwareController.h"
#include <QVBoxLayout>
#include <QLabel>

namespace HorizonUTM {

DashboardView::DashboardView(TestController* testController,
                             HardwareController* hardwareController,
                             QWidget* parent)
    : QWidget(parent)
    , m_testController(testController)
    , m_hardwareController(hardwareController)
{
    QVBoxLayout* layout = new QVBoxLayout(this);
    
    QLabel* label = new QLabel("Dashboard View - Full UI in Phase 6", this);
    label->setAlignment(Qt::AlignCenter);
    layout->addWidget(label);
}

void DashboardView::startTest() {
    // Create test with default parameters
    Test test = m_testController->createNewTest();
    test.setSampleName("Test Sample");
    test.setOperatorName("Operator");
    
    // Start test
    m_hardwareController->startTest(test);
}

} // namespace HorizonUTM
