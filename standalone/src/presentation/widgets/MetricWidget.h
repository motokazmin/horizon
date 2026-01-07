#pragma once

#include <QWidget>
#include <QLabel>

namespace HorizonUTM {

/**
 * @brief Widget for displaying a single metric value
 */
class MetricWidget : public QWidget {
    Q_OBJECT

public:
    explicit MetricWidget(const QString& label, const QString& unit, QWidget* parent = nullptr);
    
    /**
     * @brief Set the current value
     */
    void setValue(double value);
    
    /**
     * @brief Set number of decimal places
     */
    void setDecimals(int decimals) { m_decimals = decimals; }

private:
    void setupUI();
    
private:
    QString m_label;
    QString m_unit;
    int m_decimals;
    
    QLabel* m_labelWidget;
    QLabel* m_valueWidget;
};

} // namespace HorizonUTM
