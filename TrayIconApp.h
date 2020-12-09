#ifndef TRAY_ICON_APP_H
#define TRAY_ICON_APP_H

#include <QWidget>
#include <QCloseEvent>
#include <QSystemTrayIcon>

class TrayIconApp : public QWidget
{
    Q_OBJECT
public:
    explicit TrayIconApp(QWidget *parent = nullptr);
    ~TrayIconApp();

protected:
    void closeEvent(QCloseEvent *e) override; 
    void changeEvent(QEvent *e) override;

private slots:
    void slot_systemTrayIconFocused(QSystemTrayIcon::ActivationReason reason);
    void slot_show();
    void slot_exit();

private:
    QSystemTrayIcon *system_tray_icon;
    QAction *action_show;
    QAction *action_exit;
    QMenu *menu;

    void initTrayIcon();
}; // TrayIconApp

#endif // TRAY_ICON_APP_H
