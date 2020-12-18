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
};

#endif // WORK_THREAD_H
