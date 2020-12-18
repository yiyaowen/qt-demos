#ifndef THREAD_DIALOG_H
#define THREAD_DIALOG_H

#include <QDialog>
#include <QPushButton>
#include <QVector>

#include "WorkThread.h"

class ThreadDialog : public QDialog
{
    Q_OBJECT

public:
    ThreadDialog(QWidget* parent=nullptr);
    ~ThreadDialog() = default;

    void setMaxThreadCount(int maxCount);

private:
    QPushButton* btnStart;
    QPushButton* btnStop;
    QPushButton* btnQuit;

public slots:
    void slotStart();
    void slotStop();

private:
    int maxThreadCount = 1;
    QVector<WorkThread*> workThreads;
};

#endif // THREAD_DIALOG_H
