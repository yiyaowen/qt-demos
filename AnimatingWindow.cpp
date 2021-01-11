#include "AnimatingWindow.h"

AnimatingWindow::AnimatingWindow(QWidget *parent) : QWidget(parent)
{
    this->setWindowTitle("Animating QtProperties Demo");

    btnSimpleAnimation = new QPushButton("Simple Animation");
    connect(btnSimpleAnimation, SIGNAL(clicked()), this, SLOT(startSimpleAnimation()));
    btnKeyValueAnimation = new QPushButton("Key Value Animation");
    connect(btnKeyValueAnimation, SIGNAL(clicked()), this, SLOT(startKeyValueAnimation()));
    btnEasingCurvesAnimation = new QPushButton("Easing Curves Animation");
    connect(btnEasingCurvesAnimation, SIGNAL(clicked()), this, SLOT(startEasingCurvesAnimation()));
    btnParallelAnimation = new QPushButton("Parallel Animation");
    connect(btnParallelAnimation, SIGNAL(clicked()), this, SLOT(startParallelAnimation()));
    btnSequentialAnimation = new QPushButton("Sequntial Animation");
    connect(btnSequentialAnimation, SIGNAL(clicked()), this, SLOT(startSequentialAnimation()));
    btnStateAnimation = new QPushButton("State Animation");
    connect(btnStateAnimation, SIGNAL(clicked()), this, SLOT(startStateAnimation()));
    btnState = new QPushButton("State 1 to State 2");
    btnAssistState = new QPushButton("State2 to State 1");

    centerLayout = new QGridLayout;
    centerLayout->addWidget(btnSimpleAnimation, 0, 0, 1, 2);
    centerLayout->addWidget(btnKeyValueAnimation, 1, 0, 1, 2);
    centerLayout->addWidget(btnEasingCurvesAnimation, 2, 0, 1, 2);
    centerLayout->addWidget(btnParallelAnimation, 3, 0, 1, 2);
    centerLayout->addWidget(btnSequentialAnimation, 4, 0, 1, 2);
    centerLayout->addWidget(btnStateAnimation, 5, 0, 1, 2);
    centerLayout->addWidget(btnState, 6, 0, 1, 1);
    centerLayout->addWidget(btnAssistState, 6, 1, 1, 1);

    this->setLayout(centerLayout);
}

void AnimatingWindow::startSimpleAnimation()
{
    animation = new QPropertyAnimation(this, "geometry");
    animation->setDuration(5000);

    animation->setStartValue(QRect(0, 0, 100, 30));
    animation->setEndValue(QRect(250, 250, 100, 30));

    animation->start();
}

void AnimatingWindow::startKeyValueAnimation()
{
    animation = new QPropertyAnimation(this, "geometry");
    animation->setDuration(5000);

    animation->setKeyValueAt(0, QRect(0, 0, 100, 30));
    animation->setKeyValueAt(0.8, QRect(250, 250, 100, 30));
    animation->setKeyValueAt(1, QRect(0, 0, 100, 30));

    animation->start();
}

void AnimatingWindow::startEasingCurvesAnimation()
{
    animation = new QPropertyAnimation(this, "geometry");
    animation->setDuration(3000);

    animation->setStartValue(QRect(0, 0, 100, 30));
    animation->setEndValue(QRect(250, 250, 100, 30));

    animation->setEasingCurve(QEasingCurve::OutBounce);

    animation->start();
}

void AnimatingWindow::startParallelAnimation()
{
    assistWidget = new QWidget;
    assistWidget->setFixedSize(this->size());
    assistWidget->show();

    animation = new QPropertyAnimation(this, "geometry");
    animation->setDuration(5000);
    animation->setStartValue(QRect(0, 0, 100, 30));
    animation->setEndValue(QRect(250, 250, 100, 30));

    assistAnimation = new QPropertyAnimation(assistWidget, "geometry");
    assistAnimation->setDuration(4000);
    assistAnimation->setStartValue(QRect(250, 250, 100, 30));
    assistAnimation->setEndValue(QRect(0, 0, 100, 30));

    parallelGroup = new QParallelAnimationGroup;
    parallelGroup->addAnimation(animation);
    parallelGroup->addAnimation(assistAnimation);

    parallelGroup->start();
}

void AnimatingWindow::startSequentialAnimation()
{
    assistWidget = new QWidget;
    assistWidget->setFixedSize(this->size());
    assistWidget->show();

    animation = new QPropertyAnimation(this, "geometry");
    animation->setDuration(5000);
    animation->setStartValue(QRect(0, 0, 100, 30));
    animation->setEndValue(QRect(250, 250, 100, 30));

    assistAnimation = new QPropertyAnimation(assistWidget, "geometry");
    assistAnimation->setDuration(4000);
    assistAnimation->setStartValue(QRect(250, 250, 100, 30));
    assistAnimation->setEndValue(QRect(0, 0, 100, 30));

    sequentialGroup = new QSequentialAnimationGroup;
    sequentialGroup->addAnimation(animation);
    sequentialGroup->addAnimation(assistAnimation);

    sequentialGroup->start();
}

void AnimatingWindow::startStateAnimation()
{
    stateMachine = new QStateMachine;
    state = new QState(stateMachine);
    state->assignProperty(this, "geometry", QRect(0, 0, 100, 30));
    stateMachine->setInitialState(state);

    assistState = new QState(stateMachine);
    assistState->assignProperty(this, "geometry", QRect(250, 250, 100, 30));

    transition = state->addTransition(btnState, SIGNAL(clicked()), assistState);
    transition->addAnimation(new QPropertyAnimation(this, "geometry"));

    assistTransition = assistState->addTransition(btnAssistState, SIGNAL(clicked()), state);
    assistTransition->addAnimation(new QPropertyAnimation(this, "geometry"));

    stateMachine->start();
}

AnimatingWindow::~AnimatingWindow()
{
}
