#include <QtDebug>

#include "WorkThread.h"

void WorkThread::run()
{
    while (keepRun) {
        for (int n = 0; n < 10; ++n) {
            qDebug() << n << n << n << n << n << n << n << n;
        }
    }
}
