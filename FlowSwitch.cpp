#include "FlowSwitch.h"

#include <QPainter>
#include <QDateTime>

#include <QDebug>

constexpr static float empty = 0.0f;
constexpr static float full = 1.0f;

constexpr static int defaultColor[4] = { 200, 200, 200, 200 };
constexpr static int flowColor[4] = { 80, 140, 70, 255 };

FlowSwitch::FlowSwitch(QWidget *parent)
    : QWidget(parent), m_state(empty), board(new Board(this)), block(new Block(board)), timer(new QTimer)
{
    this->setFixedSize(90, 50);
    this->move((parent->width() - this->width()) / 2, (parent->height() - this->height()) / 2);

    connect(timer, SIGNAL(timeout()), this, SLOT(Update()));

    this->show();
}

FlowSwitch::~FlowSwitch()
{
    // Notice the delete order when there is
    // a parent-children releationship between
    // two widgets. The children should be
    // released first, and then the parent.
    delete block;
    delete board;
}

void FlowSwitch::paintEvent(QPaintEvent */*event*/)
{
    QPainter painter(this);
    painter.fillRect(0, 0, this->width(), this->height(), QBrush(QColor(255, 255, 255, 0)));
    painter.end();
}

void FlowSwitch::mouseReleaseEvent(QMouseEvent */*event*/)
{
    if (m_state == empty)
        TurnOnAnimated();
    else if (m_state == full)
        TurnOffAnimated();
}

void Board::paintEvent(QPaintEvent */*event*/)
{
    float ratio = state();
    int fillLength = (int)(ratio * (width() - 50));
    QPainter painter(this);
    painter.fillRect(0, 0, width(), height(), QBrush(QColor(255, 255, 255, 0)));
    painter.setPen(Qt::NoPen);

    painter.setBrush(QBrush(QColor(flowColor[0], flowColor[1], flowColor[2])));
    painter.drawPie(0, 0, 50, 50, 16 * 90, 16 * 180);
    painter.setBrush(QBrush(QColor(defaultColor[0], defaultColor[1], defaultColor[2])));
    painter.drawPie(width() - 50, 0, 50, 50, 16 * 270, 16 * 180);

    painter.setBrush(QBrush(QColor(flowColor[0], flowColor[1], flowColor[2])));
    painter.drawRect(25, 0, fillLength, height());
    painter.setBrush(QBrush(QColor(defaultColor[0], defaultColor[1], defaultColor[2])));
    painter.drawRect(fillLength + 25, 0, width() - 50 - fillLength, height());
    painter.end();
}

void Block::paintEvent(QPaintEvent */*event*/)
{
    QPainter painter(this);
    painter.setPen(Qt::NoPen);
    painter.setBrush(QBrush(QColor(150, 200, 200)));
    painter.drawRoundedRect(0, 0, this->width(), this->height(), 25, 25);
    painter.end();
}

void FlowSwitch::TurnOn()
{
    Ratio(full);
}

void FlowSwitch::TurnOff()
{
    Ratio(empty);
}

float calculateDelta(float value)
{
    if (qAbs(value - 0.5) >= 0.4)
        return 0.1f;
    else
        return (0.4 - qAbs(value - 0.5)) * 5 + 0.1f;
}

void FlowSwitch::Update()
{
    if (isAnimating) return;
    isAnimating = true;
    if (isOpening)
        m_state += calculateDelta(m_state);
    else if (isClosing)
        m_state -= calculateDelta(m_state);

    Ratio(qMin(full, qMax(empty, m_state)));

    if (m_state == empty || m_state == full) {
        isOpening = false;
        isClosing = false;
        timer->stop();
    }
    isAnimating = false;
}

void FlowSwitch::TurnOnAnimated()
{
    isOpening = true;
    isClosing =  false;
    if (m_state == empty) {
        timer->setInterval(50);
        timer->start();
    }
}

void FlowSwitch::TurnOffAnimated()
{
    isOpening = false;
    isClosing =  true;
    if (m_state == full) {
        timer->setInterval(50);
        timer->start();
    }
}

void FlowSwitch::Ratio(float ratio)
{
    m_state = ratio;
    block->Move();
    board->ChangeColor();
}

Board::Board(QWidget *parent)
    : QWidget(parent)
{
    this->setFixedSize(90,50);
    this->move(0, 0);
    ChangeColor();
    this->show();
}

float Board::state()
{
    return ((FlowSwitch*)this->parentWidget())->state();
}

void Board::ChangeColor()
{
    this->update();
}

float Block::state()
{
    return ((Board*)this->parentWidget())->state();
}

Block::Block(QWidget *parent)
    : QWidget(parent)
{
    this->setFixedSize(50, 50);
    this->move(0, 0);
    Move();
    this->show();
}

void Block::Move()
{
    int width = this->parentWidget()->width();
    int offset = (int)(state() * (width - this->width()));
    this->move(offset, 0);
    this->update();
}
