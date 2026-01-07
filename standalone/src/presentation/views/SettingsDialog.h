#pragma once

#include <QDialog>

namespace HorizonUTM {

class SettingsDialog : public QDialog {
    Q_OBJECT

public:
    explicit SettingsDialog(QWidget* parent = nullptr);
};

} // namespace HorizonUTM
