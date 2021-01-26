#ifndef CLOCK_MAIN_WINDOW_H
#define CLOCK_MAIN_WINDOW_H

#include <QMainWindow>
#include <QSystemTrayIcon>
#include <QCloseEvent>

#include <../Smallclock_Base/App_Data_Save.h>
#include "../Smallclock_Base/Command_Do_In_Thread.h"
#include "Message_Window_Show.h"

#include "clock_setting_dialog.h"
#include "new_alarm_clock_dialog.h"

QT_BEGIN_NAMESPACE
namespace Ui { class Clock_Main_Window; }
QT_END_NAMESPACE

class Clock_Main_Window : public QMainWindow
{
    Q_OBJECT

public:
    Clock_Main_Window(QWidget *parent = nullptr);
    ~Clock_Main_Window();

private slots:
    void on_action_set_triggered();

    void systemTrayMenu_Button_show_or_hide_clicked();
    void systemTrayMenu_Button_exit_clicked();

    void on_action_aboutSmallclock_triggered();

private:
    Ui::Clock_Main_Window *ui;

    Clock_Setting_Dialog *m_clock_setting_dialog;

    QSystemTrayIcon* m_system_tray;
    QMenu* m_menu_systemTray;
    QAction* m_action_menu_systemTray_show_hide;
    QAction* m_action_menu_systemTray_exit;

    App_Data_Save m_save_data = App_Data_Save(".Smallclock_Data");

    __int128 get_tv_time_now();

    void stopwatch_start();
    void stopwatch_stop();
    void stopwatch_flush();

    void mainWindow_show();
    void mainWindow_hide();

    void closeEvent(QCloseEvent *event);

    void save_data();
    void read_data();
};
#endif // CLOCK_MAIN_WINDOW_H
