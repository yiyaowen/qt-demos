#include <QApplication>

#include "Vie.h"
#include "Viewer.h"

#include <iostream>

int main(int argc, char *argv[])
{
    if (argc != 2) {
        std::cout << "Usage: exe [filename]" << std::endl;
        return VIEWER_EXIT_FAILED;
    }
    
    QApplication a(argc, argv);

    Viewer v(argv[1]);
    if (v.readFailed()) {
        std::cout << "Read target record file failed." << std::endl;
        return VIEWER_BAD_FILENAME;
    }
    v.show();

    if (a.exec() != 0) {
        std::cout << "Exit by accident." << std::endl;
        return VIEWER_EXIT_FAILED;
    }
    else {
        std::cout << "Exit OK." << std::endl;
        return VIEWER_EXIT_OK;
    }
}
