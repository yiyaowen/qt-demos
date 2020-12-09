#include <QApplication>
#include <QWidget>

#include "VirtualKeyboard.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    VirtualKeyboard *vk = new VirtualKeyboard();
    vk->show();
    
    return a.exec();
}
