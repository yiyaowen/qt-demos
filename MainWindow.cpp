#include "MainWindow.h"
#include "./ui_MainWindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    w = new QWidget;
    w->setLayout(ui->gridLayout);
    this->setCentralWidget(w);
}

MainWindow::~MainWindow()
{
    delete ui;
    delete w;
}

