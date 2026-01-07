#pragma once

#include <QDialog>
#include <QLabel>
#include <QTextEdit>
#include "domain/entities/Test.h"

namespace HorizonUTM {

class RealtimeChartWidget;

/**
 * @brief Dialog for viewing detailed test information
 */
class TestDetailsDialog : public QDialog {
    Q_OBJECT

public:
    explicit TestDetailsDialog(const Test& test, QWidget* parent = nullptr);

private:
    void setupUI();
    void populateData();
    void plotTestData();
    
private:
    Test m_test;
    
    // UI Components
    RealtimeChartWidget* m_chartWidget;
    
    // Info labels
    QLabel* m_sampleNameLabel;
    QLabel* m_operatorLabel;
    QLabel* m_dateLabel;
    QLabel* m_durationLabel;
    QLabel* m_statusLabel;
    
    // Geometry
    QLabel* m_widthLabel;
    QLabel* m_thicknessLabel;
    QLabel* m_gaugeLengthLabel;
    QLabel* m_crossSectionLabel;
    
    // Results
    QLabel* m_elasticModulusLabel;
    QLabel* m_yieldStressLabel;
    QLabel* m_ultimateStressLabel;
    QLabel* m_breakStressLabel;
    QLabel* m_elongationLabel;
    
    // Notes
    QTextEdit* m_notesEdit;
};

} // namespace HorizonUTM
