#include "TestDetailsDialog.h"
#include "../widgets/RealtimeChartWidget.h"

#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGridLayout>
#include <QGroupBox>
#include <QPushButton>
#include <QFont>

namespace HorizonUTM {

TestDetailsDialog::TestDetailsDialog(const Test& test, QWidget* parent)
    : QDialog(parent)
    , m_test(test)
{
    setWindowTitle(QString("Test Details - %1").arg(test.getSampleName()));
    setMinimumSize(900, 700);

    setupUI();
    populateData();
    plotTestData();
}

void TestDetailsDialog::setupUI() {
    QVBoxLayout* mainLayout = new QVBoxLayout(this);

    // Chart
    QGroupBox* chartGroup = new QGroupBox("Stress-Strain Curve", this);
    QVBoxLayout* chartLayout = new QVBoxLayout(chartGroup);

    m_chartWidget = new RealtimeChartWidget(this);
    chartLayout->addWidget(m_chartWidget);

    mainLayout->addWidget(chartGroup, 3);

    // Info section
    QHBoxLayout* infoLayout = new QHBoxLayout();

    // Left column - Basic info
    QGroupBox* basicGroup = new QGroupBox("Basic Information", this);
    QGridLayout* basicLayout = new QGridLayout(basicGroup);

    basicLayout->addWidget(new QLabel("Sample:", this), 0, 0);
    m_sampleNameLabel = new QLabel(this);
    QFont boldFont = m_sampleNameLabel->font();
    boldFont.setBold(true);
    m_sampleNameLabel->setFont(boldFont);
    basicLayout->addWidget(m_sampleNameLabel, 0, 1);

    basicLayout->addWidget(new QLabel("Operator:", this), 1, 0);
    m_operatorLabel = new QLabel(this);
    basicLayout->addWidget(m_operatorLabel, 1, 1);

    basicLayout->addWidget(new QLabel("Date:", this), 2, 0);
    m_dateLabel = new QLabel(this);
    basicLayout->addWidget(m_dateLabel, 2, 1);

    basicLayout->addWidget(new QLabel("Duration:", this), 3, 0);
    m_durationLabel = new QLabel(this);
    basicLayout->addWidget(m_durationLabel, 3, 1);

    basicLayout->addWidget(new QLabel("Status:", this), 4, 0);
    m_statusLabel = new QLabel(this);
    basicLayout->addWidget(m_statusLabel, 4, 1);

    infoLayout->addWidget(basicGroup);

    // Middle column - Geometry
    QGroupBox* geomGroup = new QGroupBox("Sample Geometry", this);
    QGridLayout* geomLayout = new QGridLayout(geomGroup);

    geomLayout->addWidget(new QLabel("Width:", this), 0, 0);
    m_widthLabel = new QLabel(this);
    geomLayout->addWidget(m_widthLabel, 0, 1);

    geomLayout->addWidget(new QLabel("Thickness:", this), 1, 0);
    m_thicknessLabel = new QLabel(this);
    geomLayout->addWidget(m_thicknessLabel, 1, 1);

    geomLayout->addWidget(new QLabel("Gauge Length:", this), 2, 0);
    m_gaugeLengthLabel = new QLabel(this);
    geomLayout->addWidget(m_gaugeLengthLabel, 2, 1);

    geomLayout->addWidget(new QLabel("Cross Section:", this), 3, 0);
    m_crossSectionLabel = new QLabel(this);
    m_crossSectionLabel->setFont(boldFont);
    geomLayout->addWidget(m_crossSectionLabel, 3, 1);

    infoLayout->addWidget(geomGroup);

    // Right column - Results
    QGroupBox* resultsGroup = new QGroupBox("Test Results", this);
    QGridLayout* resultsLayout = new QGridLayout(resultsGroup);

    resultsLayout->addWidget(new QLabel("Elastic Modulus:", this), 0, 0);
    m_elasticModulusLabel = new QLabel(this);
    m_elasticModulusLabel->setFont(boldFont);
    resultsLayout->addWidget(m_elasticModulusLabel, 0, 1);

    resultsLayout->addWidget(new QLabel("Yield Stress:", this), 1, 0);
    m_yieldStressLabel = new QLabel(this);
    resultsLayout->addWidget(m_yieldStressLabel, 1, 1);

    resultsLayout->addWidget(new QLabel("Ultimate Stress:", this), 2, 0);
    m_ultimateStressLabel = new QLabel(this);
    m_ultimateStressLabel->setFont(boldFont);
    resultsLayout->addWidget(m_ultimateStressLabel, 2, 1);

    resultsLayout->addWidget(new QLabel("Break Stress:", this), 3, 0);
    m_breakStressLabel = new QLabel(this);
    resultsLayout->addWidget(m_breakStressLabel, 3, 1);

    resultsLayout->addWidget(new QLabel("Elongation:", this), 4, 0);
    m_elongationLabel = new QLabel(this);
    m_elongationLabel->setFont(boldFont);
    resultsLayout->addWidget(m_elongationLabel, 4, 1);

    infoLayout->addWidget(resultsGroup);

    mainLayout->addLayout(infoLayout, 1);

    // Notes
    QGroupBox* notesGroup = new QGroupBox("Notes", this);
    QVBoxLayout* notesLayout = new QVBoxLayout(notesGroup);

    m_notesEdit = new QTextEdit(this);
    m_notesEdit->setReadOnly(true);
    m_notesEdit->setMaximumHeight(80);
    notesLayout->addWidget(m_notesEdit);

    mainLayout->addWidget(notesGroup);

    // Close button
    QHBoxLayout* buttonLayout = new QHBoxLayout();
    buttonLayout->addStretch();

    QPushButton* closeBtn = new QPushButton("Close", this);
    connect(closeBtn, &QPushButton::clicked, this, &QDialog::accept);
    buttonLayout->addWidget(closeBtn);

    mainLayout->addLayout(buttonLayout);
}

void TestDetailsDialog::populateData() {
    // Basic info
    m_sampleNameLabel->setText(m_test.getSampleName());
    m_operatorLabel->setText(m_test.getOperatorName());
    m_dateLabel->setText(m_test.getStartTime().toString("yyyy-MM-dd HH:mm:ss"));
    m_durationLabel->setText(QString::number(m_test.getDuration(), 'f', 1) + " s");

    QString status = testStatusToString(m_test.getStatus());
    m_statusLabel->setText(status);
    if (m_test.getStatus() == TestStatus::Completed) {
        m_statusLabel->setStyleSheet("color: green; font-weight: bold;");
    } else if (m_test.getStatus() == TestStatus::Failed) {
        m_statusLabel->setStyleSheet("color: red; font-weight: bold;");
    }

    // Geometry
    m_widthLabel->setText(QString::number(m_test.getWidth(), 'f', 2) + " mm");
    m_thicknessLabel->setText(QString::number(m_test.getThickness(), 'f', 2) + " mm");
    m_gaugeLengthLabel->setText(QString::number(m_test.getGaugeLength(), 'f', 1) + " mm");
    m_crossSectionLabel->setText(QString::number(m_test.getCrossSectionArea(), 'f', 2) + " mm²");

    // Results
    m_elasticModulusLabel->setText(QString::number(m_test.getResult().elasticModulus / 1000.0, 'f', 2) + " GPa");
    m_yieldStressLabel->setText(QString::number(m_test.getResult().yieldStress, 'f', 2) + " MPa");
    m_ultimateStressLabel->setText(QString::number(m_test.getResult().ultimateStress, 'f', 2) + " MPa");
    m_breakStressLabel->setText(QString::number(m_test.getResult().breakStress, 'f', 2) + " MPa");
    m_elongationLabel->setText(QString::number(m_test.getResult().elongationAtBreak, 'f', 2) + " %");

    // Notes
    m_notesEdit->setPlainText(m_test.getNotes());
}

void TestDetailsDialog::plotTestData() {
    // Clear existing data
    m_chartWidget->clearData();

    // Get all data points for this test
    const QVector<QPair<double, double>>& stressStrainData = m_test.getStressStrainData();

    // Plot each point
    for (const auto& point : stressStrainData) {
        m_chartWidget->addDataPoint(point.first, point.second); // strain, stress
    }
}

} // namespace HorizonUTM
