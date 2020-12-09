#ifndef VIRTUAL_KEYBOARD_H
#define VIRTUAL_KEYBOARD_H

#include <QApplication>
#include <QStackedWidget>
#include <QPushButton>
#include <QWidget>
#include <QMouseEvent>

#include <QVector>
#include <QMap>

class VirtualKeyboard : public QWidget
{
    Q_OBJECT
public:
    explicit VirtualKeyboard(QWidget *parent = nullptr);
    ~VirtualKeyboard();

protected:
    void mouseMoveEvent(QMouseEvent *e) override;
    void mousePressEvent(QMouseEvent *e) override;
    void mouseReleaseEvent(QMouseEvent *e) override;
    void showEvent(QShowEvent *e) override;
    void hideEvent(QHideEvent *e) override;

private slots:
    void slot_keyClicked();

private:
    void initWindow();
    void initForm();

    int key_size;
    int screen_width;
    int screen_height;
    int frame_width;
    int frame_height;

    bool is_moving;
    QPoint last_pos;

    QString current_type;
    void changeType(QString type);
    bool current_cap;
    void changeCap(bool is_upper);

    QStackedWidget *key_window;
    QWidget *alphabet_window;
    QWidget *symbol_window;

    QPushButton *close_key;
    QPushButton *type_key;

    QVector<QPushButton *> number_keys;
    QVector<QPushButton *> alphabet_keys;
    QMap<QString, QPushButton *> symbol_keys;
    QMap<QString, QPushButton *> function_keys;
}; // VirtualKeyboard

#endif // VIRTUAL_KEYBOARD_H
