#pragma once

#include <QWidget>
#include "src/third_party/qcustomplot/qcustomplot.h"
#include "domain/value_objects/SensorData.h"

namespace HorizonUTM {

/**
 * @brief Real-time stress-strain chart widget
 */
class RealtimeChartWidget : public QWidget {
    Q_OBJECT

public:
    explicit RealtimeChartWidget(QWidget* parent = nullptr);
    ~RealtimeChartWidget() override = default;
    
    /**
     * @brief Add data point to chart
     */
    void addDataPoint(double strain, double stress);
    
    /**
     * @brief Clear all data
     */
    void clearData();
    
    /**
     * @brief Set axis labels
     */
    void setLabels(const QString& xLabel, const QString& yLabel);

public slots:
    /**
     * @brief Update chart with new sensor data
     */
    void onSensorDataReceived(const SensorData& data);

private:
    void setupPlot();
    void updateAxisRanges();
    
private:
    QCustomPlot* m_plot;
    QVector<double> m_strainData;
    QVector<double> m_stressData;
    
    double m_maxStrain;
    double m_maxStress;
};

} // namespace HorizonUTM
