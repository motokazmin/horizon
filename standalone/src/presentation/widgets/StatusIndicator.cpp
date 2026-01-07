#include "StatusIndicator.h"
#include <QPainter>
#include <QHBoxLayout>

namespace HorizonUTM {

StatusIndicator::StatusIndicator(QWidget* parent)
    : QWidget(parent)
    , m_connected(false)
{
    QHBoxLayout* layout = new QHBoxLayout(this);
    layout->setContentsMargins(0, 0, 0, 0);

    m_label = new QLabel("Disconnected", this);
    layout->addWidget(m_label);

    setFixedSize(120, 24);
}

void StatusIndicator::setConnected(bool connected) {
    if (m_connected != connected) {
        m_connected = connected;
        m_label->setText(connected ? "Connected" : "Disconnected");
        update();
    }
}

void StatusIndicator::paintEvent(QPaintEvent* /*event*/) {
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);

    // Draw LED indicator
    QRect ledRect(5, 4, 16, 16);

    QColor ledColor = m_connected ? QColor(0, 200, 0) : QColor(200, 0, 0);

    // Outer glow
    painter.setPen(Qt::NoPen);
    painter.setBrush(ledColor.lighter(120));
    painter.drawEllipse(ledRect.adjusted(-2, -2, 2, 2));

    // LED body
    painter.setBrush(ledColor);
    painter.drawEllipse(ledRect);

    // Highlight
    QRadialGradient gradient(ledRect.center(), ledRect.width() / 2);
    gradient.setColorAt(0, QColor(255, 255, 255, 180));
    gradient.setColorAt(0.5, QColor(255, 255, 255, 0));
    painter.setBrush(gradient);
    painter.drawEllipse(ledRect);
}

QSize StatusIndicator::sizeHint() const {
    return QSize(120, 24);
}

} // namespace HorizonUTM
