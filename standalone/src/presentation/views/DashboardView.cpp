#include "DashboardView.h"
#include "TestConfigDialog.h"
#include "application/controllers/TestController.h"
#include "application/controllers/HardwareController.h"
#include <QVBoxLayout>
#include <QLabel>
#include <QMessageBox>

namespace HorizonUTM {

DashboardView::DashboardView(TestController* testController,
                             HardwareController* hardwareController,
                             QWidget* parent)
    : QWidget(parent)
    , m_testController(testController)
    , m_hardwareController(hardwareController)
{
    QVBoxLayout* layout = new QVBoxLayout(this);

    QLabel* label = new QLabel("Dashboard View - Chart will be added in Phase 1.2", this);
    label->setAlignment(Qt::AlignCenter);
    layout->addWidget(label);
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

} // namespace HorizonUTM
