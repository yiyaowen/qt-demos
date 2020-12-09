#include <QScreen>
#include <QGridLayout>

#include "VirtualKeyboard.h"

#ifdef DEBUG
#include <iostream>
using namespace std;
#endif

VirtualKeyboard::VirtualKeyboard(QWidget *parent) : QWidget(parent)
{
    QScreen *screen = QGuiApplication::primaryScreen();
    screen_width = screen->availableGeometry().width();
    screen_height = screen->availableGeometry().height() +
        screen->availableGeometry().y();
    key_size = screen_width / 20;

    initWindow();
    initForm();
}

VirtualKeyboard::~VirtualKeyboard() { }

void VirtualKeyboard::initWindow()
{
    this->setWindowFlags(Qt::WindowStaysOnTopHint | Qt::FramelessWindowHint);
    this->setFixedSize(20 * key_size, 5 * key_size);
    frame_width = this->width();
    frame_height = this->height();
    //this->setFocusPolicy(Qt::NoFocus);

    key_window = new QStackedWidget(this);
    key_window->setFixedSize(20 * key_size, 3 * key_size);
    alphabet_window = new QWidget();
    symbol_window = new QWidget();

    close_key = new QPushButton(this);
    close_key->setText("(X)");
    close_key->setFixedSize(2 * key_size, key_size);

    type_key = new QPushButton(this);
    type_key->setText("123");
    type_key->setFixedSize(3 * key_size, key_size);

    // number & alphabet keys
    for (int i = 0; i < 10; ++i) {
        QPushButton *temp = new QPushButton(this);
        temp->setText(QString::asprintf("%d", i));
        temp->setFixedSize(2 * key_size, key_size);
        number_keys.append(temp);
    }

    for (char c = 'A'; c <= 'Z'; ++c) {
        QPushButton *temp = new QPushButton(this);
        temp->setText(QString::asprintf("%c", c));
        temp->setFixedSize(2 * key_size, key_size);
        alphabet_keys.append(temp);
    }

    // symbol keys
    QVector<QString> symbol_names = {
        "hyphen", "slash", "colon", "semicolon", "l_parenthesis",
        "r_parenthesis", "currency", "and", "at", "d_quotation_mark", "period", "comma",
        "question_mark", "exclamation_mark", "s_quotation_mark", "TLD", "asterisk",
        "sharp"
    };
    QVector<QString> symbol_texts = {
        "-", "/", ":", ";", "(", ")", "$", "&&", "@", "\"", ".", ",", "?", "!", "'",
        ".com", "*", "#"
    };
    QMap<QString, QString> symbol_map;
    for (int i = 0; i < qMin(symbol_names.size(), symbol_texts.size()); ++i) {
        symbol_map[symbol_names[i]] = symbol_texts[i];
    }

    for (auto name : symbol_map.keys()) {
        QPushButton *temp = new QPushButton(this);
        temp->setText(symbol_map[name]);
        temp->setFixedSize(2 * key_size, key_size);
        symbol_keys.insert(name, temp);
    }

    // function keys
    QVector<QString> function_names = {
        "cap_off", "delete", "carriage_return", "space"
    };
    QVector<QString> function_texts = {
        "Cap/Off", "<==", "new line", "space"
    };
    QMap<QString, QString> function_map;
    for (int i = 0; i < qMin(function_names.size(), function_texts.size()); ++i) {
        function_map[function_names[i]] = function_texts[i];
    }
    for (auto name : function_map.keys()) {
        QPushButton *temp = new QPushButton(this);
        temp->setText(function_map[name]);
        temp->setFixedSize(2 * key_size, key_size);
        function_keys.insert(name, temp);
    }

    // resize special keys
    type_key->setFixedSize(3 * key_size, key_size);
    function_keys["space"]->setFixedSize(14 * key_size, key_size);
    function_keys["carriage_return"]->setFixedSize(3 * key_size, key_size);

    // alphabet window
    QGridLayout *alphabet_layout = new QGridLayout(this);
    for (int i = 0; i < 10; ++i) {
        alphabet_layout->addWidget(alphabet_keys[i], 0, i*2, 1, 2, Qt::AlignCenter);
    }
    for (int i = 0; i < 9; ++i) {
        alphabet_layout->addWidget(alphabet_keys[i+10], 1, i*2+1, 1, 2, Qt::AlignCenter);
    }
    alphabet_layout->addWidget(function_keys.value("cap_off"), 2, 0, 1, 2, Qt::AlignCenter);
    for (int i = 0; i < 7; ++i) {
        alphabet_layout->addWidget(alphabet_keys[i+19], 2, i*2+3, 1, 2, Qt::AlignCenter);
    }
    alphabet_layout->addWidget(function_keys["delete"], 2, 18, 1, 2, Qt::AlignCenter);
    alphabet_layout->setSpacing(0);
    alphabet_layout->setContentsMargins(0, 0, 0, 0);
    alphabet_window->setLayout(alphabet_layout);

    // symbol window
    QGridLayout *symbol_layout = new QGridLayout(this);
    for (int i = 0; i < 10; ++i) {
        symbol_layout->addWidget(number_keys[(i+1)%10], 0, i*2, 1, 2, Qt::AlignCenter);
    }
    for (int i = 0; i < 9; ++i) {
        symbol_layout->addWidget(symbol_keys[symbol_names[i]], 1, i*2+1, 1, 2, Qt::AlignCenter);
    }
    for (int i = 0; i < 9; ++i) {
        symbol_layout->addWidget(symbol_keys[symbol_names[i+9]], 2, i*2+1, 1, 2, Qt::AlignCenter);
    }
    symbol_layout->setSpacing(0);
    symbol_layout->setContentsMargins(0, 0, 0, 0);
    symbol_window->setLayout(symbol_layout);

    // key window
    key_window->insertWidget(0, alphabet_window);
    key_window->insertWidget(1, symbol_window);
    key_window->setCurrentIndex(0);

    QGridLayout *layout = new QGridLayout(this);
    layout->addWidget(close_key, 0, 0, 1, 2, Qt::AlignCenter);
    layout->addWidget(key_window, 1, 0, 3, 20, Qt::AlignCenter);
    layout->addWidget(type_key, 4, 0, 1, 3, Qt::AlignCenter);
    layout->addWidget(function_keys["space"], 4, 3, 1, 14, Qt::AlignCenter);
    layout->addWidget(function_keys["carriage_return"], 4, 17, 1, 3, Qt::AlignCenter);
    layout->setSpacing(0);
    layout->setContentsMargins(0, 0, 0, 0);
    this->setLayout(layout);
}

void VirtualKeyboard::initForm()
{
    current_type = "alp",
    current_cap = false;

    QList<QPushButton *> keys = this->findChildren<QPushButton *>();
    for (auto key : keys) {
        connect(key, SIGNAL(clicked()), this, SLOT(slot_keyClicked()));
    }
}

void VirtualKeyboard::showEvent(QShowEvent *e)
{
#ifdef DEBUG
    cout << "Show event triggered" << endl;
#endif
    QWidget::showEvent(e);
    // bottom center
    QPoint target_point = QPoint(screen_width / 2 - frame_width / 2, 
        screen_height - frame_height);
    this->move(target_point);
}

void VirtualKeyboard::hideEvent(QHideEvent *e)
{
#ifdef DEBUG
    cout << "Hide event triggered" << endl;
#endif
    QWidget::hideEvent(e);
    current_type = "alp",
    changeType(current_type);
    current_cap = false;
    changeCap(current_cap);
    key_window->setCurrentIndex(0);
}

void VirtualKeyboard::slot_keyClicked()
{
    // TODO
    QPushButton * source_key = qobject_cast<QPushButton *>(this->sender());
    QString text = source_key->text();
    if (text == "(X)") {
        // go to backstage
        this->hide();
    }
    else if (text == "123") {
        key_window->setCurrentIndex(1);
        type_key->setText("abc");
    }
    else if (text == "abc") {
        key_window->setCurrentIndex(0);
        type_key->setText("123");
    }
}

void VirtualKeyboard::changeType(QString type)
{
    // TODO
}

void VirtualKeyboard::changeCap(bool isUpper)
{
    // TODO
}

void VirtualKeyboard::mousePressEvent(QMouseEvent *e)
{
    if (e->button() == Qt::LeftButton) {
        is_moving = true;
        last_pos = e->globalPos() - this->pos();
    }
}

void VirtualKeyboard::mouseMoveEvent(QMouseEvent *e)
{
    // use manhattan length, i.e. |deltaX| + |deltaY|
    if (is_moving && (e->buttons() & Qt::LeftButton) &&
        (e->globalPos() - last_pos).manhattanLength() > QApplication::startDragDistance())
    {
        this->move(e->globalPos() - last_pos);
        last_pos = e->globalPos() - pos();
    }
}

void VirtualKeyboard::mouseReleaseEvent(QMouseEvent *e)
{
    Q_UNUSED(e);
    is_moving = false;
}
