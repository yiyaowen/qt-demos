#include <QAction>
#include <QMenu>
#ifdef DEBUG
#include <iostream>
using namespace std;
#endif

#include "TrayIconApp.h"

TrayIconApp::TrayIconApp(QWidget *parent) : QWidget(parent)
{
#ifdef DEBUG
    cout << boolalpha << QSystemTrayIcon::isSystemTrayAvailable() << endl;
#endif
    initTrayIcon();
}

TrayIconApp::~TrayIconApp() { }

void TrayIconApp::slot_systemTrayIconFocused(QSystemTrayIcon::ActivationReason reason)
{
    switch (reason)
    {
    case QSystemTrayIcon::Trigger:
        system_tray_icon->showMessage("Message Title", "Message Content",
            QSystemTrayIcon::Information, 10000);
        return;
    
    case QSystemTrayIcon::DoubleClick:
        this->show();
        this->setWindowState((this->windowState() & ~Qt::WindowMinimized) | Qt::WindowActive);
        system_tray_icon->hide();
        return;

    case QSystemTrayIcon::MiddleClick:
        // TODO
        return;

    case QSystemTrayIcon::Context:
        // TODO
        return;

    case QSystemTrayIcon::Unknown:
        // TODO
        return;
    }
}

void TrayIconApp::closeEvent(QCloseEvent *e)
{
#ifdef DEBUG
    cout << "Close event triggered" << endl;
#endif
    QWidget::closeEvent(e);
    system_tray_icon->show();
}

void TrayIconApp::changeEvent(QEvent *e)
{
#ifdef DEBUG
    cout << "Change event triggered" << endl;
#endif
    QWidget::changeEvent(e);
    if (e->type() == QEvent::WindowStateChange &&
        (this->windowState() & Qt::WindowMinimized))
    {
        this->hide();
        system_tray_icon->show();
    }
}

void TrayIconApp::initTrayIcon()
{
    system_tray_icon = new QSystemTrayIcon();
    system_tray_icon->setIcon(QIcon("../vk.png"));
    system_tray_icon->setToolTip("Click to active virtual keyboard!");

    // action
    action_show = new QAction("Show", this);
    connect(action_show, SIGNAL(triggered()), this, SLOT(slot_show()));
    action_exit = new QAction("Exit", this);
    connect(action_exit, SIGNAL(triggered()), this, SLOT(slot_exit()));

    // menu
    menu = new QMenu(this);
    menu->addAction(action_show);
    menu->addSeparator();
    menu->addAction(action_exit);
    system_tray_icon->setContextMenu(menu);

    connect(system_tray_icon, SIGNAL(activated(QSystemTrayIcon::ActivationReason)),
        this, SLOT(slot_systemTrayIconFocused(QSystemTrayIcon::ActivationReason)));
}

void TrayIconApp::slot_show()
{
    this->show();
    this->setWindowState((this->windowState() & ~Qt::WindowMinimized) | Qt::WindowActive);
    system_tray_icon->hide();
}

void TrayIconApp::slot_exit()
{
    exit(0);
}
