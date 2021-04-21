#ifndef FLOWSWITCH_H
#define FLOWSWITCH_H

#include <QWidget>
#include <QTimer>

class Block : public QWidget
{
    Q_OBJECT
public:
    explicit Block(QWidget *parent = nullptr);

    void Move();

    inline float state();

private:
    void paintEvent(QPaintEvent *event) override;
};

class Board : public QWidget
{
    Q_OBJECT
public:
    explicit Board(QWidget *parent = nullptr);

    void ChangeColor();

    inline float state();

private:
    QWidget left, right;

    void paintEvent(QPaintEvent *event) override;
};

class FlowSwitch : public QWidget
{
    Q_OBJECT
public:
    explicit FlowSwitch(QWidget *parent = nullptr);
    ~FlowSwitch();

    void TurnOn();
    void TurnOff();

    void TurnOnAnimated();
    void TurnOffAnimated();

    inline float state() { return m_state; }

private:
    void paintEvent(QPaintEvent *event) override;

    void mouseReleaseEvent(QMouseEvent *event) override;

    void Ratio(float ratio);

private slots:
    void Update();

private:
    float m_state;
    bool isOpening = false;
    bool isClosing = false;

    Board *board;
    Block *block;

    QTimer *timer;

    bool isAnimating = false;
};


#endif // FLOWSWITCH_H
