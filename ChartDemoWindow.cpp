#include "ChartDemoWindow.h"

#include <QtCharts>
using namespace QtCharts;

ChartDemoWindow::ChartDemoWindow(QWidget* parent)
    : QMainWindow(parent)
{
    createChart();
}

ChartDemoWindow::~ChartDemoWindow()
{
}

void ChartDemoWindow::createChart()
{
    QChartView* chartView = new QChartView(this);
    QChart* chart = new QChart;
    chart->setTitle("简单函数曲线");
    chartView->setChart(chart);
    this->setCentralWidget(chartView);

    QLineSeries* series0 = new QLineSeries;
    QLineSeries* series1 = new QLineSeries;
    series0->setName("Sin 曲线");
    series1->setName("Cos 曲线");
    chart->addSeries(series0);
    chart->addSeries(series1);

    qreal t = 0, y1, y2, deltaT = 0.1;
    int count = 100;
    for (int i = 0; i < count; ++i) {
        y1 = qSin(t);
        series0->append(t, y1);
        y2 = qSin(t+20);
        series1->append(t, y2);
        t += deltaT;
    }

    chart->createDefaultAxes();
    auto* axisX = chart->axes(Qt::Horizontal).first();
    auto* axisY = chart->axes(Qt::Vertical).first();
    axisX->setRange(0, 10);
    axisX->setTitleText("time(secs)");
    axisY->setRange(-2, 2);
    axisY->setTitleText("value");
}

