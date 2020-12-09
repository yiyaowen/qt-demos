#ifndef SHAPE_WIDGET_H
#define SHAPE_WIDGET_H

#include <QWidget>
#include <QMouseEvent>
#include <QPaintEvent>

class ShapeWidget : public QWidget
{
    Q_OBJECT
public:
    ShapeWidget(QWidget *parent = nullptr);
    ~ShapeWidget();

protected:
    void mousePressEvent(QMouseEvent *e) override;
    void mouseMoveEvent(QMouseEvent *e) override;
    void paintEvent(QPaintEvent *e) override;

private:
    QPoint dragPosition;
};

#endif // SHAPE_WIDGET_H
