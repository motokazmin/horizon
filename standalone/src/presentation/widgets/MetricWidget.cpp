#include "MetricWidget.h"
#include <QVBoxLayout>
#include <QFrame>

namespace HorizonUTM {

MetricWidget::MetricWidget(const QString& label, const QString& unit, QWidget* parent)
    : QWidget(parent)
    , m_label(label)
    , m_unit(unit)
    , m_decimals(2)
{
    setupUI();
}

void MetricWidget::setupUI() {
    QVBoxLayout* layout = new QVBoxLayout(this);
    layout->setContentsMargins(10, 10, 10, 10);
    layout->setSpacing(5);
    
    // Frame for visual separation
    QFrame* frame = new QFrame(this);
    frame->setFrameShape(QFrame::StyledPanel);
    frame->setFrameShadow(QFrame::Raised);
    frame->setLineWidth(2);
    
    QVBoxLayout* frameLayout = new QVBoxLayout(frame);
    frameLayout->setContentsMargins(15, 10, 15, 10);
    frameLayout->setSpacing(2);
    
    // Label
    m_labelWidget = new QLabel(m_label, frame);
    QFont labelFont = m_labelWidget->font();
    labelFont.setPointSize(9);
    labelFont.setBold(false);
    m_labelWidget->setFont(labelFont);
    m_labelWidget->setAlignment(Qt::AlignCenter);
    m_labelWidget->setStyleSheet("color: #666666;");
    frameLayout->addWidget(m_labelWidget);
    
    // Value
    m_valueWidget = new QLabel("0.00", frame);
    QFont valueFont = m_valueWidget->font();
    valueFont.setPointSize(24);
    valueFont.setBold(true);
    m_valueWidget->setFont(valueFont);
    m_valueWidget->setAlignment(Qt::AlignCenter);
    m_valueWidget->setStyleSheet("color: #0066CC;");
    frameLayout->addWidget(m_valueWidget);
    
    // Unit
    QLabel* unitLabel = new QLabel(m_unit, frame);
    QFont unitFont = unitLabel->font();
    unitFont.setPointSize(10);
    unitLabel->setFont(unitFont);
    unitLabel->setAlignment(Qt::AlignCenter);
    unitLabel->setStyleSheet("color: #888888;");
    frameLayout->addWidget(unitLabel);
    
    layout->addWidget(frame);
    
    setMinimumWidth(120);
    setMinimumHeight(100);
}

void MetricWidget::setValue(double value) {
    m_valueWidget->setText(QString::number(value, 'f', m_decimals));
}

} // namespace HorizonUTM
