#include "RealtimeChartWidget.h"
#include "core/Logger.h"
#include <QVBoxLayout>

namespace HorizonUTM {

RealtimeChartWidget::RealtimeChartWidget(QWidget* parent)
    : QWidget(parent)
    , m_plot(nullptr)
    , m_maxStrain(0.0)
    , m_maxStress(0.0)
{
    m_plot = new QCustomPlot(this);
    
    QVBoxLayout* layout = new QVBoxLayout(this);
    layout->setContentsMargins(0, 0, 0, 0);
    layout->addWidget(m_plot);
    
    setupPlot();
}

void RealtimeChartWidget::setupPlot() {
    // Add graph
    m_plot->addGraph();
    m_plot->graph(0)->setPen(QPen(QColor(0, 100, 200), 2));
    m_plot->graph(0)->setLineStyle(QCPGraph::lsLine);
    m_plot->graph(0)->setScatterStyle(QCPScatterStyle(QCPScatterStyle::ssNone));
    
    // Set axis labels
    m_plot->xAxis->setLabel("Strain (%)");
    m_plot->yAxis->setLabel("Stress (MPa)");
    
    // Set axis label fonts
    QFont labelFont;
    labelFont.setPointSize(11);
    labelFont.setBold(true);
    m_plot->xAxis->setLabelFont(labelFont);
    m_plot->yAxis->setLabelFont(labelFont);
    
    // Set axis tick label fonts
    QFont tickFont;
    tickFont.setPointSize(9);
    m_plot->xAxis->setTickLabelFont(tickFont);
    m_plot->yAxis->setTickLabelFont(tickFont);
    
    // Enable grid
    m_plot->xAxis->grid()->setVisible(true);
    m_plot->yAxis->grid()->setVisible(true);
    m_plot->xAxis->grid()->setPen(QPen(QColor(200, 200, 200), 1, Qt::DotLine));
    m_plot->yAxis->grid()->setPen(QPen(QColor(200, 200, 200), 1, Qt::DotLine));
    
    // Set initial ranges
    m_plot->xAxis->setRange(0, 10);
    m_plot->yAxis->setRange(0, 100);
    
    // Enable interactions
    m_plot->setInteractions(QCP::iRangeDrag | QCP::iRangeZoom);
    m_plot->axisRect()->setRangeDrag(Qt::Horizontal | Qt::Vertical);
    m_plot->axisRect()->setRangeZoom(Qt::Horizontal | Qt::Vertical);
    
    // Set background
    m_plot->setBackground(QBrush(Qt::white));
    m_plot->axisRect()->setBackground(QBrush(QColor(250, 250, 250)));
    
    m_plot->replot();
}

void RealtimeChartWidget::addDataPoint(double strain, double stress) {
    m_strainData.append(strain);
    m_stressData.append(stress);
    
    // Update max values
    if (strain > m_maxStrain) m_maxStrain = strain;
    if (stress > m_maxStress) m_maxStress = stress;
    
    // Update graph data
    m_plot->graph(0)->setData(m_strainData, m_stressData);
    
    // Update axis ranges with some padding
    updateAxisRanges();
    
    // Replot only every 10 points for performance
    if (m_strainData.size() % 10 == 0) {
        m_plot->replot();
    }
}

void RealtimeChartWidget::clearData() {
    m_strainData.clear();
    m_stressData.clear();
    m_maxStrain = 0.0;
    m_maxStress = 0.0;
    
    m_plot->graph(0)->data()->clear();
    m_plot->xAxis->setRange(0, 10);
    m_plot->yAxis->setRange(0, 100);
    m_plot->replot();
    
    LOG_DEBUG("Chart data cleared");
}

void RealtimeChartWidget::setLabels(const QString& xLabel, const QString& yLabel) {
    m_plot->xAxis->setLabel(xLabel);
    m_plot->yAxis->setLabel(yLabel);
    m_plot->replot();
}

void RealtimeChartWidget::onSensorDataReceived(const SensorData& data) {
    // Add point to chart
    addDataPoint(data.strain, data.stress);
}

void RealtimeChartWidget::updateAxisRanges() {
    // Add 10% padding to ranges
    double strainPadding = m_maxStrain * 0.1;
    double stressPadding = m_maxStress * 0.1;
    
    double xMax = m_maxStrain + strainPadding;
    double yMax = m_maxStress + stressPadding;
    
    // Set minimum ranges
    if (xMax < 1.0) xMax = 1.0;
    if (yMax < 10.0) yMax = 10.0;
    
    m_plot->xAxis->setRange(0, xMax);
    m_plot->yAxis->setRange(0, yMax);
}

} // namespace HorizonUTM
