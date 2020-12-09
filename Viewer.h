#ifndef VIEWER_H
#define VIEWER_H

#include <QWidget>
#include <QLabel>
#include <QPushButton>
#include <QPlainTextEdit>
#include <QPaintEvent>
#include <QVector>
#include <QTimer>

class Viewer : public QWidget
{
    Q_OBJECT
public:
    explicit Viewer(QString filename, QWidget *parent = nullptr);
    ~Viewer();

    bool readFailed();

protected:
    bool eventFilter(QObject *watched, QEvent *event) override;

private:
    QString filename_;
    QString stateMessage_;
    QVector<QPoint> record_;
    QTimer *timer;
    int totalStep_ = 0;
    int curStep_ = 0;
    bool curStatus_ = false;
    bool readRecord(QString filename);

    QLabel *lblBoard;
    QPushButton *btnNext, *btnPrev, *btnEnd;
    QPlainTextEdit *recorder;
    void initWindow();
    void paintBoard();
    void updateText();

private slots:
    void slot_showNextStep();
    void slot_showPrevStep();
    void slot_showEndStep();
    void slot_goToStep(QString value);
    void slot_autoPlay(int option);
};

#endif // VIEWER_H
