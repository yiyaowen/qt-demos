#include "AnimatingWindow.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    AnimatingWindow w;
    w.show();
    return a.exec();
}
