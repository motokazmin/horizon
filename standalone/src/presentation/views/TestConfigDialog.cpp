#include "TestConfigDialog.h"
#include "domain/entities/TestMethod.h"
#include "domain/services/TestMethodValidator.h"
#include "core/Logger.h"

#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QFormLayout>
#include <QGroupBox>
#include <QLabel>
#include <QPushButton>
#include <QMessageBox>

namespace HorizonUTM {

TestConfigDialog::TestConfigDialog(QWidget* parent)
    : QDialog(parent)
{
    setWindowTitle("Test Configuration");
    setMinimumWidth(500);
    
    setupUI();
    setupConnections();
    loadDefaults();
}

void TestConfigDialog::setupUI() {
    QVBoxLayout* mainLayout = new QVBoxLayout(this);
    
    // Sample Information Group
    QGroupBox* sampleGroup = new QGroupBox("Sample Information", this);
    QFormLayout* sampleLayout = new QFormLayout(sampleGroup);
    
    m_sampleNameEdit = new QLineEdit(this);
    m_sampleNameEdit->setPlaceholderText("e.g., Sample-001");
    sampleLayout->addRow("Sample Name*:", m_sampleNameEdit);
    
    m_operatorEdit = new QLineEdit(this);
    m_operatorEdit->setPlaceholderText("e.g., John Doe");
    sampleLayout->addRow("Operator*:", m_operatorEdit);
    
    mainLayout->addWidget(sampleGroup);
    
    // Test Method Group
    QGroupBox* methodGroup = new QGroupBox("Test Method", this);
    QFormLayout* methodLayout = new QFormLayout(methodGroup);
    
    m_testMethodCombo = new QComboBox(this);
    m_testMethodCombo->addItems(TestMethod::getAvailableMethods());
    methodLayout->addRow("Standard:", m_testMethodCombo);
    
    mainLayout->addWidget(methodGroup);
    
    // Geometry Group
    QGroupBox* geometryGroup = new QGroupBox("Sample Geometry", this);
    QFormLayout* geometryLayout = new QFormLayout(geometryGroup);
    
    m_widthSpin = new QDoubleSpinBox(this);
    m_widthSpin->setRange(1.0, 100.0);
    m_widthSpin->setDecimals(2);
    m_widthSpin->setSuffix(" mm");
    m_widthSpin->setValue(10.0);
    geometryLayout->addRow("Width*:", m_widthSpin);
    
    m_thicknessSpin = new QDoubleSpinBox(this);
    m_thicknessSpin->setRange(0.1, 50.0);
    m_thicknessSpin->setDecimals(2);
    m_thicknessSpin->setSuffix(" mm");
    m_thicknessSpin->setValue(4.0);
    geometryLayout->addRow("Thickness*:", m_thicknessSpin);
    
    m_gaugeLengthSpin = new QDoubleSpinBox(this);
    m_gaugeLengthSpin->setRange(10.0, 500.0);
    m_gaugeLengthSpin->setDecimals(1);
    m_gaugeLengthSpin->setSuffix(" mm");
    m_gaugeLengthSpin->setValue(50.0);
    geometryLayout->addRow("Gauge Length*:", m_gaugeLengthSpin);
    
    m_crossSectionLabel = new QLabel("40.00 mm²", this);
    QFont boldFont = m_crossSectionLabel->font();
    boldFont.setBold(true);
    m_crossSectionLabel->setFont(boldFont);
    geometryLayout->addRow("Cross Section:", m_crossSectionLabel);
    
    mainLayout->addWidget(geometryGroup);
    
    // Test Parameters Group
    QGroupBox* paramsGroup = new QGroupBox("Test Parameters", this);
    QFormLayout* paramsLayout = new QFormLayout(paramsGroup);
    
    m_speedSpin = new QDoubleSpinBox(this);
    m_speedSpin->setRange(0.1, 500.0);
    m_speedSpin->setDecimals(1);
    m_speedSpin->setSuffix(" mm/min");
    m_speedSpin->setValue(5.0);
    paramsLayout->addRow("Speed*:", m_speedSpin);
    
    m_forceLimitSpin = new QDoubleSpinBox(this);
    m_forceLimitSpin->setRange(100.0, 100000.0);
    m_forceLimitSpin->setDecimals(0);
    m_forceLimitSpin->setSuffix(" N");
    m_forceLimitSpin->setValue(10000.0);
    paramsLayout->addRow("Force Limit:", m_forceLimitSpin);
    
    m_temperatureSpin = new QDoubleSpinBox(this);
    m_temperatureSpin->setRange(-50.0, 200.0);
    m_temperatureSpin->setDecimals(1);
    m_temperatureSpin->setSuffix(" °C");
    m_temperatureSpin->setValue(23.0);
    paramsLayout->addRow("Temperature:", m_temperatureSpin);
    
    mainLayout->addWidget(paramsGroup);
    
    // Notes
    QGroupBox* notesGroup = new QGroupBox("Notes (Optional)", this);
    QVBoxLayout* notesLayout = new QVBoxLayout(notesGroup);
    
    m_notesEdit = new QTextEdit(this);
    m_notesEdit->setMaximumHeight(80);
    m_notesEdit->setPlaceholderText("Additional notes about this test...");
    notesLayout->addWidget(m_notesEdit);
    
    mainLayout->addWidget(notesGroup);
    
    // Buttons
    QHBoxLayout* buttonLayout = new QHBoxLayout();
    buttonLayout->addStretch();
    
    QPushButton* cancelBtn = new QPushButton("Cancel", this);
    connect(cancelBtn, &QPushButton::clicked, this, &QDialog::reject);
    buttonLayout->addWidget(cancelBtn);
    
    QPushButton* startBtn = new QPushButton("Start Test", this);
    startBtn->setDefault(true);
    QPalette palette = startBtn->palette();
    palette.setColor(QPalette::Button, QColor(0, 150, 0));
    palette.setColor(QPalette::ButtonText, Qt::white);
    startBtn->setPalette(palette);
    connect(startBtn, &QPushButton::clicked, this, &TestConfigDialog::onAccept);
    buttonLayout->addWidget(startBtn);
    
    mainLayout->addLayout(buttonLayout);
    
    // Info label
    QLabel* infoLabel = new QLabel("* Required fields", this);
    QFont smallFont = infoLabel->font();
    smallFont.setPointSize(smallFont.pointSize() - 1);
    infoLabel->setFont(smallFont);
    infoLabel->setStyleSheet("color: gray;");
    mainLayout->addWidget(infoLabel);
}

void TestConfigDialog::setupConnections() {
    // Update cross-section when geometry changes
    connect(m_widthSpin, QOverload<double>::of(&QDoubleSpinBox::valueChanged),
            this, [this]() {
        double area = m_widthSpin->value() * m_thicknessSpin->value();
        m_crossSectionLabel->setText(QString::number(area, 'f', 2) + " mm²");
    });
    
    connect(m_thicknessSpin, QOverload<double>::of(&QDoubleSpinBox::valueChanged),
            this, [this]() {
        double area = m_widthSpin->value() * m_thicknessSpin->value();
        m_crossSectionLabel->setText(QString::number(area, 'f', 2) + " mm²");
    });
    
    // Update defaults when test method changes
    connect(m_testMethodCombo, &QComboBox::currentTextChanged,
            this, &TestConfigDialog::onTestMethodChanged);
}

void TestConfigDialog::loadDefaults() {
    // Load default values for ISO 527-2
    onTestMethodChanged("ISO 527-2");
}

void TestConfigDialog::onTestMethodChanged(const QString& method) {
    // Load recommended values for selected method
    if (method == "ISO 527-2") {
        m_widthSpin->setValue(10.0);
        m_thicknessSpin->setValue(4.0);
        m_gaugeLengthSpin->setValue(50.0);
        m_speedSpin->setValue(5.0);
    } else if (method == "ASTM D638") {
        m_widthSpin->setValue(13.0);
        m_thicknessSpin->setValue(3.2);
        m_gaugeLengthSpin->setValue(50.0);
        m_speedSpin->setValue(5.0);
    }
}

bool TestConfigDialog::validateInputs() {
    // Check required fields
    if (m_sampleNameEdit->text().trimmed().isEmpty()) {
        QMessageBox::warning(this, "Validation Error", "Sample name is required.");
        m_sampleNameEdit->setFocus();
        return false;
    }
    
    if (m_operatorEdit->text().trimmed().isEmpty()) {
        QMessageBox::warning(this, "Validation Error", "Operator name is required.");
        m_operatorEdit->setFocus();
        return false;
    }
    
    // Validate against test method standards
    TestMethodValidator::ValidationResult result = TestMethodValidator::validateTestParameters(
        m_testMethodCombo->currentText(),
        m_widthSpin->value(),
        m_thicknessSpin->value(),
        m_gaugeLengthSpin->value(),
        m_speedSpin->value()
    );
    
    if (!result.isValid) {
        QString errorMsg = "Test parameters do not meet " + m_testMethodCombo->currentText() + " standards:\n\n";
        for (const QString& error : result.errors) {
            errorMsg += "• " + error + "\n";
        }
        
        if (!result.warnings.isEmpty()) {
            errorMsg += "\nWarnings:\n";
            for (const QString& warning : result.warnings) {
                errorMsg += "• " + warning + "\n";
            }
        }
        
        QMessageBox::StandardButton reply = QMessageBox::warning(
            this,
            "Validation Warning",
            errorMsg + "\nDo you want to proceed anyway?",
            QMessageBox::Yes | QMessageBox::No
        );
        
        return reply == QMessageBox::Yes;
    }
    
    // Show warnings if any
    if (!result.warnings.isEmpty()) {
        QString warningMsg = "Warnings:\n";
        for (const QString& warning : result.warnings) {
            warningMsg += "• " + warning + "\n";
        }
        warningMsg += "\nProceed with test?";
        
        QMessageBox::StandardButton reply = QMessageBox::information(
            this,
            "Information",
            warningMsg,
            QMessageBox::Yes | QMessageBox::No
        );
        
        return reply == QMessageBox::Yes;
    }
    
    return true;
}

void TestConfigDialog::onAccept() {
    if (validateInputs()) {
        LOG_INFO(QString("Test configured: %1 by %2")
            .arg(m_sampleNameEdit->text())
            .arg(m_operatorEdit->text()));
        accept();
    }
}

TestParametersDTO TestConfigDialog::getTestParameters() const {
    TestParametersDTO params;
    
    params.sampleName = m_sampleNameEdit->text().trimmed();
    params.operatorName = m_operatorEdit->text().trimmed();
    params.testMethod = m_testMethodCombo->currentText();
    
    params.width = m_widthSpin->value();
    params.thickness = m_thicknessSpin->value();
    params.gaugeLength = m_gaugeLengthSpin->value();
    
    params.speed = m_speedSpin->value();
    params.forceLimit = m_forceLimitSpin->value();
    params.temperature = m_temperatureSpin->value();
    
    params.notes = m_notesEdit->toPlainText().trimmed();
    
    return params;
}

void TestConfigDialog::setTestParameters(const TestParametersDTO& params) {
    m_sampleNameEdit->setText(params.sampleName);
    m_operatorEdit->setText(params.operatorName);
    
    int index = m_testMethodCombo->findText(params.testMethod);
    if (index >= 0) {
        m_testMethodCombo->setCurrentIndex(index);
    }
    
    m_widthSpin->setValue(params.width);
    m_thicknessSpin->setValue(params.thickness);
    m_gaugeLengthSpin->setValue(params.gaugeLength);
    
    m_speedSpin->setValue(params.speed);
    m_forceLimitSpin->setValue(params.forceLimit);
    m_temperatureSpin->setValue(params.temperature);
    
    m_notesEdit->setPlainText(params.notes);
}

} // namespace HorizonUTM
