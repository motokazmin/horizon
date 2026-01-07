#pragma once
#include <QWidget>
namespace HorizonUTM { 
    class MetricWidget : public QWidget {
    public:
        explicit MetricWidget(QWidget* parent = nullptr) : QWidget(parent) {}
    };
    class RealtimeChartWidget : public QWidget {
    public:
        explicit RealtimeChartWidget(QWidget* parent = nullptr) : QWidget(parent) {}
    };
}
