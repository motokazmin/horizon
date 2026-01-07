#include "ResultsView.h"
#include "TestDetailsDialog.h"
#include "application/controllers/TestController.h"
#include "core/Logger.h"

#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QHeaderView>
#include <QMessageBox>
#include <QFileDialog>

namespace HorizonUTM {

ResultsView::ResultsView(TestController* testController, QWidget* parent)
    : QWidget(parent)
    , m_testController(testController)
    , m_tableWidget(nullptr)
    , m_viewDetailsBtn(nullptr)
    , m_deleteBtn(nullptr)
    , m_exportBtn(nullptr)
    , m_refreshBtn(nullptr)
{
    setupUI();
    setupConnections();
    loadTests();
}

void ResultsView::setupUI() {
    QVBoxLayout* mainLayout = new QVBoxLayout(this);
    mainLayout->setContentsMargins(10, 10, 10, 10);

    // Table
    m_tableWidget = new QTableWidget(this);
    m_tableWidget->setColumnCount(8);
    m_tableWidget->setHorizontalHeaderLabels({
        "ID", "Date", "Sample", "Operator",
        "Max Stress (MPa)", "E (GPa)", "Elongation (%)", "Status"
    });

    // Table settings
    m_tableWidget->setSelectionBehavior(QAbstractItemView::SelectRows);
    m_tableWidget->setSelectionMode(QAbstractItemView::SingleSelection);
    m_tableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);
    m_tableWidget->setSortingEnabled(true);
    m_tableWidget->horizontalHeader()->setStretchLastSection(true);
    m_tableWidget->verticalHeader()->setVisible(false);
    m_tableWidget->setAlternatingRowColors(true);

    // Resize columns
    m_tableWidget->horizontalHeader()->setSectionResizeMode(0, QHeaderView::ResizeToContents);
    m_tableWidget->horizontalHeader()->setSectionResizeMode(1, QHeaderView::ResizeToContents);
    m_tableWidget->horizontalHeader()->setSectionResizeMode(2, QHeaderView::Stretch);
    m_tableWidget->horizontalHeader()->setSectionResizeMode(3, QHeaderView::ResizeToContents);

    mainLayout->addWidget(m_tableWidget);

    // Buttons
    QHBoxLayout* buttonLayout = new QHBoxLayout();

    m_viewDetailsBtn = new QPushButton("View Details", this);
    m_viewDetailsBtn->setEnabled(false);
    buttonLayout->addWidget(m_viewDetailsBtn);

    m_exportBtn = new QPushButton("Export Selected", this);
    m_exportBtn->setEnabled(false);
    buttonLayout->addWidget(m_exportBtn);

    m_deleteBtn = new QPushButton("Delete", this);
    m_deleteBtn->setEnabled(false);
    buttonLayout->addWidget(m_deleteBtn);

    buttonLayout->addStretch();

    m_refreshBtn = new QPushButton("Refresh", this);
    buttonLayout->addWidget(m_refreshBtn);

    mainLayout->addLayout(buttonLayout);
}

void ResultsView::setupConnections() {
    connect(m_tableWidget, &QTableWidget::itemSelectionChanged,
            this, &ResultsView::onSelectionChanged);

    // Use lambda for itemDoubleClicked to ignore the parameter
    connect(m_tableWidget, &QTableWidget::itemDoubleClicked,
            this, [this](QTableWidgetItem*) { onViewDetails(); });

    connect(m_viewDetailsBtn, &QPushButton::clicked,
            this, &ResultsView::onViewDetails);

    connect(m_deleteBtn, &QPushButton::clicked,
            this, &ResultsView::onDeleteTest);

    connect(m_exportBtn, &QPushButton::clicked,
            this, &ResultsView::onExportTest);

    connect(m_refreshBtn, &QPushButton::clicked,
            this, &ResultsView::onRefresh);
}

void ResultsView::loadTests() {
    m_tests = m_testController->getAllTests();

    m_tableWidget->setSortingEnabled(false);
    m_tableWidget->setRowCount(0);

    for (const Test& test : m_tests) {
        int row = m_tableWidget->rowCount();
        m_tableWidget->insertRow(row);

        // ID
        QTableWidgetItem* idItem = new QTableWidgetItem(QString::number(test.getId()));
        idItem->setTextAlignment(Qt::AlignCenter);
        m_tableWidget->setItem(row, 0, idItem);

        // Date
        QString dateStr = test.getStartTime().toString("yyyy-MM-dd HH:mm");
        QTableWidgetItem* dateItem = new QTableWidgetItem(dateStr);
        m_tableWidget->setItem(row, 1, dateItem);

        // Sample
        m_tableWidget->setItem(row, 2, new QTableWidgetItem(test.getSampleName()));

        // Operator
        m_tableWidget->setItem(row, 3, new QTableWidgetItem(test.getOperatorName()));

        // Max Stress
        QTableWidgetItem* stressItem = new QTableWidgetItem(
            QString::number(test.getResult().ultimateStress, 'f', 2));
        stressItem->setTextAlignment(Qt::AlignRight | Qt::AlignVCenter);
        m_tableWidget->setItem(row, 4, stressItem);

        // Elastic Modulus (convert MPa to GPa)
        QTableWidgetItem* modulusItem = new QTableWidgetItem(
            QString::number(test.getResult().elasticModulus / 1000.0, 'f', 2));
        modulusItem->setTextAlignment(Qt::AlignRight | Qt::AlignVCenter);
        m_tableWidget->setItem(row, 5, modulusItem);

        // Elongation at break
        QTableWidgetItem* elongItem = new QTableWidgetItem(
            QString::number(test.getResult().elongationAtBreak, 'f', 2));
        elongItem->setTextAlignment(Qt::AlignRight | Qt::AlignVCenter);
        m_tableWidget->setItem(row, 6, elongItem);

        // Status
        QString status = testStatusToString(test.getStatus());
        QTableWidgetItem* statusItem = new QTableWidgetItem(status);
        statusItem->setTextAlignment(Qt::AlignCenter);

        // Color code by status
        if (test.getStatus() == TestStatus::Completed) {
            statusItem->setForeground(QColor(34, 197, 94)); // green
        } else if (test.getStatus() == TestStatus::Failed) {
            statusItem->setForeground(QColor(239, 68, 68)); // red
        } else if (test.getStatus() == TestStatus::Stopped) {
            statusItem->setForeground(QColor(245, 158, 11)); // amber
        }

        m_tableWidget->setItem(row, 7, statusItem);
    }

    m_tableWidget->setSortingEnabled(true);
    m_tableWidget->sortItems(0, Qt::DescendingOrder); // Newest first

    LOG_INFO(QString("Loaded %1 tests").arg(m_tests.size()));
}

void ResultsView::refreshTestList() {
    loadTests();
    updateButtonStates();
}

void ResultsView::onViewDetails() {
    int testId = getSelectedTestId();
    if (testId < 0) return;

    qDebug() << "=== ResultsView::onViewDetails ===";
    qDebug() << "=== Loading test ID:" << testId;

    // Load full test data including data points
    Test fullTest = m_testController->getTest(testId);

    qDebug() << "=== Loaded test ID:" << fullTest.getId();
    qDebug() << "=== Sample name:" << fullTest.getSampleName();
    qDebug() << "=== Data points:" << fullTest.getDataPointCount();

    if (fullTest.getId() < 0) {
        QMessageBox::warning(this, "Error", "Failed to load test data");
        return;
    }

    // Show details dialog with full data
    TestDetailsDialog dialog(fullTest, this);
    dialog.exec();
}

void ResultsView::onDeleteTest() {
    int testId = getSelectedTestId();
    if (testId < 0) return;

    QMessageBox::StandardButton reply = QMessageBox::question(
        this,
        "Delete Test",
        "Are you sure you want to delete this test?\nThis action cannot be undone.",
        QMessageBox::Yes | QMessageBox::No
    );

    if (reply == QMessageBox::Yes) {
        if (m_testController->deleteTest(testId)) {
            LOG_INFO(QString("Test %1 deleted").arg(testId));
            refreshTestList();
        } else {
            QMessageBox::critical(this, "Error", "Failed to delete test");
        }
    }
}

void ResultsView::onExportTest() {
    int testId = getSelectedTestId();
    if (testId < 0) return;

    QString fileName = QFileDialog::getSaveFileName(
        this,
        "Export Test Data",
        QString("test_%1.csv").arg(testId),
        "CSV Files (*.csv)"
    );

    if (!fileName.isEmpty()) {
        // TODO: Export single test to CSV
        QMessageBox::information(this, "Export",
            "Single test export will be implemented in Phase 2.1");
    }
}

void ResultsView::onRefresh() {
    refreshTestList();
}

void ResultsView::onSelectionChanged() {
    updateButtonStates();
}

void ResultsView::updateButtonStates() {
    bool hasSelection = !m_tableWidget->selectedItems().isEmpty();
    m_viewDetailsBtn->setEnabled(hasSelection);
    m_deleteBtn->setEnabled(hasSelection);
    m_exportBtn->setEnabled(hasSelection);
}

int ResultsView::getSelectedTestId() const {
    QList<QTableWidgetItem*> selected = m_tableWidget->selectedItems();
    if (selected.isEmpty()) {
        return -1;
    }

    int row = selected.first()->row();
    return m_tableWidget->item(row, 0)->text().toInt();
}

} // namespace HorizonUTM
