#ifndef CHART_DEMO_WINDOW_H
#define CHART_DEMO_WINDOW_H

#include <QMainWindow>

class ChartDemoWindow : public QMainWindow
{
    Q_OBJECT
    
public:
    ChartDemoWindow(QWidget* parent=nullptr);
    ~ChartDemoWindow();

private:
    void createChart();
};

#endif // CHART_DEMO_WINDOW_H
