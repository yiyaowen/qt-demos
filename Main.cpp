#include <QApplication>

#include "TrayIconApp.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    a.setQuitOnLastWindowClosed(false);

    TrayIconApp *tia = new TrayIconApp();
    tia->show();

    return a.exec();
}
