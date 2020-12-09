#include <QPainter>
#include <QPixmap>
#include <QBitmap>

#include "ShapeWidget.h"

ShapeWidget::ShapeWidget(QWidget *parent) : QWidget(parent)
{
    this->setWindowFlags(Qt::FramelessWindowHint);
    this->setAttribute(Qt::WA_TranslucentBackground);
    QPixmap pix;
    pix.load("../sword.png", 0, Qt::AvoidDither | Qt::ThresholdDither | Qt::ThresholdAlphaDither);
    this->resize(pix.size());
    this->setMask(QBitmap(pix.mask()));
}

ShapeWidget::~ShapeWidget() { }

void ShapeWidget::mousePressEvent(QMouseEvent *e)
{
    if (e->button() == Qt::LeftButton) {
        dragPosition = e->globalPos() - this->frameGeometry().topLeft();
        e->accept();
    }
    if (e->button() == Qt::RightButton) {
        close();
    }
}

void ShapeWidget::mouseMoveEvent(QMouseEvent *e)
{
    if (e->buttons() & Qt::LeftButton) {
        this->move(e->globalPos() - dragPosition);
        e->accept();
    }
}

void ShapeWidget::paintEvent(QPaintEvent *e)
{
    QPainter painter(this);
    painter.drawPixmap(0, 0, QPixmap("../sword.png"));
}
