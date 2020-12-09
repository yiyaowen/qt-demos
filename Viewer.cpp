#include <QComboBox>
#include <QPainter>
#include <QImage>
#include <QPoint>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QFile>
#include <QLineEdit>

#include "Viewer.h"

Viewer::Viewer(QString filename, QWidget *parent) : QWidget(parent)
{
    filename_ = filename;
    curStatus_ = readRecord(filename_);

    timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(slot_showNextStep()));

    initWindow();
    lblBoard->setFixedSize(530, 530);
    lblBoard->installEventFilter(this);
    this->setWindowTitle(tr("Gobang Viewer Widget"));

    recorder->clear();
    stateMessage_ = QString::asprintf(
        tr("Read file %s ").toStdString().c_str(),
        filename_.mid(filename_.lastIndexOf("/")+1).toStdString().c_str()
    );
    if (curStatus_) {
        stateMessage_.append(tr("successful."));
    }
    else {
        stateMessage_.append(tr("failed."));
    }
    recorder->appendPlainText(stateMessage_);
}

Viewer::~Viewer() { }

bool Viewer::readFailed()
{
    return !curStatus_;
}

void Viewer::initWindow()
{
    lblBoard = new QLabel;
    btnNext = new QPushButton(tr("Next"));
    btnPrev = new QPushButton(tr("Prev"));
    connect(btnNext, SIGNAL(clicked()), this, SLOT(slot_showNextStep()));
    connect(btnPrev, SIGNAL(clicked()), this, SLOT(slot_showPrevStep()));
    QLabel *lblStep = new QLabel(tr("Go to"));
    lblStep->setAlignment(Qt::AlignRight | Qt::AlignVCenter);
    QLineEdit *leStep = new QLineEdit;
    leStep->setValidator(new QIntValidator(0, 15*15, leStep));
    connect(leStep, SIGNAL(textChanged(QString)), this, SLOT(slot_goToStep(QString)));
    btnEnd = new QPushButton(tr("End"));
    connect(btnEnd, SIGNAL(clicked()), this, SLOT(slot_showEndStep()));
    QLabel *lblAuto = new QLabel(tr("Auto play"));
    lblAuto->setAlignment(Qt::AlignRight | Qt::AlignVCenter);
    QComboBox *cbAuto = new QComboBox;
    cbAuto->addItem(tr("Disabled"));
    for (int i = 1; i <= 5; ++i) {
        cbAuto->addItem(QString::asprintf(tr("Every %d sec").toStdString().c_str(), i));
    }
    connect(cbAuto, SIGNAL(currentIndexChanged(int)), this, SLOT(slot_autoPlay(int)));
    recorder = new QPlainTextEdit;
    recorder->setReadOnly(true);
    QHBoxLayout *layout = new QHBoxLayout;
    QHBoxLayout *btnLayout = new QHBoxLayout;
    QVBoxLayout *sceneLayout = new QVBoxLayout;

    btnLayout->addWidget(btnNext);
    btnLayout->addWidget(btnPrev);
    btnLayout->addWidget(lblStep);
    btnLayout->addWidget(leStep);
    btnLayout->addWidget(btnEnd);
    btnLayout->addWidget(lblAuto);
    btnLayout->addWidget(cbAuto);
    QWidget *btnLayoutWidget = new QWidget;
    btnLayoutWidget->setLayout(btnLayout);
    sceneLayout->addWidget(lblBoard);
    sceneLayout->addWidget(btnLayoutWidget);
    QWidget *sceneLayoutWidget = new QWidget;
    sceneLayoutWidget->setLayout(sceneLayout);
    layout->addWidget(sceneLayoutWidget);
    layout->addWidget(recorder);

    this->setLayout(layout);
}

bool Viewer::eventFilter(QObject *watched, QEvent *event)
{
    if (watched == lblBoard && event->type() == QEvent::Paint) {
        paintBoard();
    }
    return QWidget::eventFilter(watched, event);
}

bool Viewer::readRecord(QString filename)
{
    int row, index;
    row = 0;
    totalStep_ = 0;

    record_.clear();
    record_.resize(15*15);

    QFile file(filename); 
    if (file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        while (!file.atEnd()) {
            QString line = file.readLine();
            line = line.mid(0, line.indexOf("\n"));
            QStringList lineList = line.split(" ");
            for (int col = 0; col < lineList.size(); ++col) {
                if ((index = qAbs(lineList[col].toInt())) != 0) {
                    ++totalStep_;
                    if (index > 15*15) {
                        return false;
                    }
                    record_[index-1] = QPoint(34*col+18, 34*row+5);
                }
            }
            ++row;
        }
        return true;
    }
    return false;
}

void Viewer::paintBoard()
{
    QPainter painter(lblBoard);
    painter.drawImage(QPoint(0, 0), QImage(":/board.png"));
    for (int i = 0; i < curStep_; ++i) {
        painter.drawImage(record_[i],
            (i % 2 == 0) ? 
            QImage(":/black_cir.png") : 
            QImage(":/white_cir.png")
        );
        painter.setPen((i % 2 == 0) ? Qt::white : Qt::black);
        int fontSize = (i+1 > 99) ? 15 : (i+1 > 9) ? 18 : 22;
        QFont numFont; numFont.setPixelSize(fontSize); numFont.setBold(true);
        painter.setFont(numFont);
        int offsetX = (i+1 > 99) ? 2 : (i+1 > 9) ? 5 : 9;
        int offsetY = (i+1 > 99) ? 20 : (i+1 > 9) ? 22 : 24;
        painter.drawText(QPoint(record_[i].x()+offsetX, record_[i].y()+offsetY),
            QString("%1").arg(i+1));
    }
    updateText(); 
}

void Viewer::updateText()
{
    recorder->clear();
    recorder->appendPlainText(stateMessage_);
    for (int i = 0; i < curStep_; ++i) {
        recorder->appendPlainText(QString("Step %1: (%2, %3)").arg(i+1)
            .arg((char)((record_[i].x()-18)/34 + 'A')).arg(15 - (record_[i].y()-5)/34));
    }
}

void Viewer::slot_showNextStep()
{
    if (curStep_ < totalStep_) {
        ++curStep_;
        this->repaint();
    }
}

void Viewer::slot_showPrevStep()
{
    if (curStep_ > 0) {
        --curStep_;
        this->repaint();
    }
}

void Viewer::slot_goToStep(QString value)
{
    curStep_ = qMin(totalStep_, value.toInt());
    this->repaint();
}

void Viewer::slot_showEndStep()
{
    curStep_ = totalStep_;
    this->repaint();
}

void Viewer::slot_autoPlay(int option)
{
    if (option == 0) {
        timer->stop();
        btnNext->setEnabled(true);
        btnPrev->setEnabled(true);
        btnEnd->setEnabled(true);
    }
    else {
        btnNext->setEnabled(false);
        btnPrev->setEnabled(false);
        btnEnd->setEnabled(false);
        timer->start(1000*option);
    }
}
