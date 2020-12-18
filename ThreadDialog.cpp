#include <QHBoxLayout>

#include "ThreadDialog.h"

ThreadDialog::ThreadDialog(QWidget* parent)
    : QDialog(parent)
{
    this->setWindowTitle("Multithreading Demo");
    btnStart = new QPushButton("Start");
    btnStop = new QPushButton("Stop");
    btnQuit = new QPushButton("Quit");

    QHBoxLayout* layout = new QHBoxLayout;
    layout->addWidget(btnStart);
    layout->addWidget(btnStop);
    layout->addWidget(btnQuit);

    this->setLayout(layout);

    connect(btnStart, SIGNAL(clicked()), this, SLOT(slotStart()));
    connect(btnStop, SIGNAL(clicked()), this, SLOT(slotStop()));
    connect(btnQuit, SIGNAL(clicked()), this, SLOT(close()));
}

void ThreadDialog::slotStart()
{
    btnStart->setEnabled(false);

    workThreads.clear();
    for (int i = 0; i < maxThreadCount; ++i) {
        workThreads.append(new WorkThread);
    }
    for (auto workThread : workThreads) {
        workThread->start();
    }

    btnStop->setEnabled(true);
}

void ThreadDialog::slotStop()
{
    btnStop->setEnabled(false);

    for (auto workThread : workThreads) {
        workThread->stopRun();
    }

    btnStart->setEnabled(true);
}

void ThreadDialog::setMaxThreadCount(int maxCount)
{
    maxThreadCount = maxCount;
}
