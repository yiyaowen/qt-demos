#ifndef ANIMATING_WINDOW_H
#define ANIMATING_WINDOW_H

#include <QWidget>
#include <QPushButton>
#include <QGridLayout>
#include <QPropertyAnimation>
#include <QParallelAnimationGroup>
#include <QSequentialAnimationGroup>
#include <QStateMachine>
#include <QState>
#include <QSignalTransition>

class AnimatingWindow : public QWidget
{
    Q_OBJECT

public:
    AnimatingWindow(QWidget *parent = nullptr);
    ~AnimatingWindow();

private:
    QGridLayout* centerLayout;
    QPushButton* btnSimpleAnimation;
    QPushButton* btnKeyValueAnimation;
    QPushButton* btnEasingCurvesAnimation;
    QPushButton* btnParallelAnimation;
    QPushButton* btnSequentialAnimation;
    QPushButton* btnStateAnimation;
    QPushButton* btnState, * btnAssistState;
    QWidget* assistWidget;

    QParallelAnimationGroup* parallelGroup;
    QSequentialAnimationGroup* sequentialGroup;
    QPropertyAnimation* animation;
    QPropertyAnimation* assistAnimation;
    QStateMachine* stateMachine;
    QState* state, * assistState;
    QSignalTransition* transition, * assistTransition;

private slots:
    void startSimpleAnimation();
    void startKeyValueAnimation();
    void startEasingCurvesAnimation();
    void startParallelAnimation();
    void startSequentialAnimation();
    void startStateAnimation();
};
#endif // ANIMATING_WINDOW_H
