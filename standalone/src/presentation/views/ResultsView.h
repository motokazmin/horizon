#pragma once

#include <QWidget>
#include <QTableWidget>
#include <QPushButton>
#include <QDateTime>
#include "domain/entities/Test.h"

namespace HorizonUTM {

class TestController;

/**
 * @brief View for browsing completed tests
 */
class ResultsView : public QWidget {
    Q_OBJECT

public:
    explicit ResultsView(TestController* testController, QWidget* parent = nullptr);
    
    /**
     * @brief Refresh the test list
     */
    void refreshTestList();

private slots:
    void onViewDetails();
    void onDeleteTest();
    void onExportTest();
    void onRefresh();
    void onSelectionChanged();

private:
    void setupUI();
    void setupConnections();
    void loadTests();
    void updateButtonStates();
    
    int getSelectedTestId() const;
    
private:
    TestController* m_testController;
    
    // UI Components
    QTableWidget* m_tableWidget;
    QPushButton* m_viewDetailsBtn;
    QPushButton* m_deleteBtn;
    QPushButton* m_exportBtn;
    QPushButton* m_refreshBtn;
    
    QList<Test> m_tests;
};

} // namespace HorizonUTM
