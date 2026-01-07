#include "SampleQueueView.h"
#include <QVBoxLayout>
#include <QLabel>

namespace HorizonUTM {

SampleQueueView::SampleQueueView(TestController* testController, QWidget* parent)
    : QWidget(parent)
    , m_testController(testController)
{
    QVBoxLayout* layout = new QVBoxLayout(this);
    
    QLabel* label = new QLabel("Sample Queue View - Full UI in Phase 6", this);
    label->setAlignment(Qt::AlignCenter);
    layout->addWidget(label);
}

} // namespace HorizonUTM
