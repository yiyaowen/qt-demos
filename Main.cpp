#include <QApplication>

#include "ShapeWidget.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    ShapeWidget *sw = new ShapeWidget();
    sw->show();

    return a.exec();
}
