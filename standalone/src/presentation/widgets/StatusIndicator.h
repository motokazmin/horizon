#pragma once

#include <QWidget>
#include <QLabel>

namespace HorizonUTM {

/**
 * @brief Visual status indicator (LED-style)
 */
class StatusIndicator : public QWidget {
    Q_OBJECT

public:
    explicit StatusIndicator(QWidget* parent = nullptr);
    
    void setConnected(bool connected);
    bool isConnected() const { return m_connected; }

protected:
    void paintEvent(QPaintEvent* event) override;
    QSize sizeHint() const override;

private:
    bool m_connected;
    QLabel* m_label;
};

} // namespace HorizonUTM
