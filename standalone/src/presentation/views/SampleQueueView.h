#pragma once

#include <QWidget>

namespace HorizonUTM {

class TestController;

class SampleQueueView : public QWidget {
    Q_OBJECT

public:
    explicit SampleQueueView(TestController* testController, QWidget* parent = nullptr);

private:
    TestController* m_testController;
};

} // namespace HorizonUTM
