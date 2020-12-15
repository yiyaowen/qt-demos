#include "ChartDemoWindow.h"

#include <QApplication>

int main(int argc, char* argv[])
{
    QApplication a(argc, argv);

    ChartDemoWindow w;
    w.show();

    return a.exec();
}
