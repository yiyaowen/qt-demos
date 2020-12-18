#ifndef WORK_THREAD_H
#define WORK_THREAD_H

#include <QThread>

class WorkThread : public QThread
{
    Q_OBJECT

public:
    WorkThread() = default;
    ~WorkThread() = default;

protected:
    void run() override;

public:
    void stopRun() { keepRun = false; }

private:
    bool keepRun = true;
};

#endif // WORK_THREAD_H
