#include <QApplication>

#include <cstdlib>

#include "ThreadDialog.h"

int main(int argc, char* argv[])
{
    QApplication a(argc, argv);

    ThreadDialog d;
    if (argc == 2) {
        d.setMaxThreadCount(atoi(argv[1]));
    }
    d.show();

    return a.exec();
}
