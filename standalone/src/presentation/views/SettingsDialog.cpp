#include "SettingsDialog.h"
#include <QVBoxLayout>
#include <QLabel>
#include <QPushButton>

namespace HorizonUTM {

SettingsDialog::SettingsDialog(QWidget* parent)
    : QDialog(parent)
{
    setWindowTitle("Settings");
    resize(400, 300);
    
    QVBoxLayout* layout = new QVBoxLayout(this);
    
    QLabel* label = new QLabel("Settings Dialog - Full UI in Phase 6", this);
    label->setAlignment(Qt::AlignCenter);
    layout->addWidget(label);
    
    QPushButton* closeBtn = new QPushButton("Close", this);
    connect(closeBtn, &QPushButton::clicked, this, &QDialog::accept);
    layout->addWidget(closeBtn);
}

} // namespace HorizonUTM
