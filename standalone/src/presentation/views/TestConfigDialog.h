#pragma once

#include <QDialog>
#include <QLineEdit>
#include <QDoubleSpinBox>
#include <QComboBox>
#include <QTextEdit>
#include "application/dto/TestParametersDTO.h"
#include <QLabel>

namespace HorizonUTM {

/**
 * @brief Dialog for configuring test parameters
 */
class TestConfigDialog : public QDialog {
    Q_OBJECT

public:
    explicit TestConfigDialog(QWidget* parent = nullptr);
    
    /**
     * @brief Get configured test parameters
     */
    TestParametersDTO getTestParameters() const;
    
    /**
     * @brief Set initial values
     */
    void setTestParameters(const TestParametersDTO& params);

private slots:
    /**
     * @brief Validate and accept dialog
     */
    void onAccept();
    
    /**
     * @brief Handle test method change
     */
    void onTestMethodChanged(const QString& method);

private:
    void setupUI();
    void setupConnections();
    void loadDefaults();
    bool validateInputs();
    
private:
    // Sample info
    QLineEdit* m_sampleNameEdit;
    QLineEdit* m_operatorEdit;
    
    // Test method
    QComboBox* m_testMethodCombo;
    
    // Geometry
    QDoubleSpinBox* m_widthSpin;
    QDoubleSpinBox* m_thicknessSpin;
    QDoubleSpinBox* m_gaugeLengthSpin;
    
    // Test parameters
    QDoubleSpinBox* m_speedSpin;
    QDoubleSpinBox* m_forceLimitSpin;
    QDoubleSpinBox* m_temperatureSpin;
    
    // Notes
    QTextEdit* m_notesEdit;
    
    // Calculated values
    QLabel* m_crossSectionLabel;
};

} // namespace HorizonUTM
