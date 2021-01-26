#include <iostream>
#include <thread>
#include <chrono>
#include <signal.h>
#include <unistd.h>
#include <fstream>
#include <iomanip>

#include <sys/prctl.h>
#include <sys/time.h>

#include <QMessageBox>
#include <QPushButton>
#include <QTreeWidgetItemIterator>

#include "clocktimer_choose_time_dialog.h"
#include "clock_about_dialog.h"

#include "clock_main_window.h"
#include "./ui_clock_main_window.h"

#include "../../config.h"

using namespace std;

bool m_is_mainWindow_show = true;

const string m_const_string_Smallclock_version =
        "<Smallclock_version=" +
        to_string(APP_VERSION_MAJOR) +
        "." +
        to_string(APP_VERSION_MINOR) +
        "." +
        to_string(APP_VERSION_REVISION) +
        ">";

Clock_Main_Window::Clock_Main_Window(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::Clock_Main_Window)
{
    ui->setupUi(this);

    setWindowIcon(QIcon(":/program_icons/SmallClock.png"));

    m_clock_setting_dialog = new Clock_Setting_Dialog();
    m_clock_setting_dialog->hide();


    m_menu_systemTray = new QMenu(this);

    m_action_menu_systemTray_show_hide = new QAction(m_menu_systemTray);
    m_action_menu_systemTray_show_hide->setText("隐藏");
    connect(m_action_menu_systemTray_show_hide, SIGNAL(triggered()), this,
            SLOT(systemTrayMenu_Button_show_or_hide_clicked()));

    m_action_menu_systemTray_exit = new QAction(m_menu_systemTray);
    m_action_menu_systemTray_exit->setText("退出");
    connect(m_action_menu_systemTray_exit, SIGNAL(triggered()), this,
            SLOT(systemTrayMenu_Button_exit_clicked()));

    m_menu_systemTray->addAction(m_action_menu_systemTray_show_hide);
    m_menu_systemTray->addAction(m_action_menu_systemTray_exit);

    m_system_tray = new QSystemTrayIcon(this);
    m_system_tray->setContextMenu(m_menu_systemTray);
    m_system_tray->setToolTip("SystemTray Program");
    m_system_tray->setIcon(QIcon(":/program_icons/SmallClock_SystemTray.png"));
    m_system_tray->show();

    read_data();

    if(m_clock_setting_dialog->get_is_hide_when_app_start() == true)
    {
        mainWindow_hide();
    }
    else
    {
        mainWindow_show();
    }
}

Clock_Main_Window::~Clock_Main_Window()
{
    delete ui;

    delete m_action_menu_systemTray_show_hide;
    delete m_action_menu_systemTray_exit;
    delete m_menu_systemTray;
    delete m_system_tray;
}

void Clock_Main_Window::mainWindow_show()
{
    m_is_mainWindow_show = true;
    QMainWindow::show();
    m_action_menu_systemTray_show_hide->setText("隐藏");
}
void Clock_Main_Window::mainWindow_hide()
{
    m_is_mainWindow_show = false;
    QMainWindow::hide();
    m_action_menu_systemTray_show_hide->setText("显示");
}

void Clock_Main_Window::closeEvent(QCloseEvent *event)
{
    //保存数据
    save_data();

    if(event->spontaneous() == true) //事件来自底层窗口系统（直接按窗口关闭按钮）
    {
        if(m_clock_setting_dialog->get_is_hide_when_mainWindow_clockButton_click() == true)
        {
            event->ignore();
            this->mainWindow_hide();
            return;
        }
    }

    if(m_clock_setting_dialog->get_is_sendTips_when_window_will_be_close() == true)
    {
        mainWindow_show();
        QMessageBox message_box(QMessageBox::Icon::NoIcon,
                                "Small Clock 即将关闭",
                                "您确定要继续吗？",
                                QMessageBox::StandardButton::Ok | QMessageBox::StandardButton::Cancel,
                                this,
                                Qt::Dialog | Qt::MSWindowsFixedSizeDialogHint);

        if(message_box.exec() == QMessageBox::StandardButton::Cancel)
        {
            event->ignore();
        }
    }
}

void Clock_Main_Window::save_data()
{
    m_save_data.start_write();

    m_save_data.write_next_data(m_const_string_Smallclock_version);

    m_clock_setting_dialog->get_main_timer_form_setting().write_save_data(&m_save_data);

    m_save_data.write_next_data<bool>(m_clock_setting_dialog->get_is_sendTips_when_window_will_be_close());
    m_save_data.write_next_data<bool>(m_clock_setting_dialog->get_is_hide_when_mainWindow_clockButton_click());
    m_save_data.write_next_data<bool>(m_clock_setting_dialog->get_is_hide_when_app_start());

    ui->clock_alarmClock_form->get_data().write_save_data(&m_save_data);

    m_save_data.finish_write();
}
void Clock_Main_Window::read_data()
{
    m_save_data.start_read();

    string smallclock_version = m_save_data.read_next_data();
    if(smallclock_version == m_const_string_Smallclock_version)
    {
        Clock_Main_Timer_Form::Timer_Form_Data timer_data;
        timer_data.load_save_data(&m_save_data);
        ui->clock_timer_form->set_data(timer_data);

        m_clock_setting_dialog
                ->set_is_sendTips_when_window_will_be_close(m_save_data.read_next_data<bool>());
        m_clock_setting_dialog
                ->set_is_hide_when_mainWindow_clockButton_click(m_save_data.read_next_data<bool>());
        m_clock_setting_dialog->set_is_hide_when_app_start(m_save_data.read_next_data<bool>());

        Clock_Main_AlarmClock_Form::AlarmClock_Form_Data alarmClock_data;
        alarmClock_data.load_save_data(&m_save_data);
        ui->clock_alarmClock_form->set_data(alarmClock_data);
    }

    m_save_data.finish_read();
}



void Clock_Main_Window::on_action_set_triggered()
{
    m_clock_setting_dialog->show_main_timer_form_setting(
                ui->clock_timer_form->get_data()
                );
    m_clock_setting_dialog->show_alarm_clock_default_setting(
                ui->clock_alarmClock_form->get_data().alarm_clock_default_setting
                );

    m_clock_setting_dialog->exec();

    ui->clock_timer_form->set_data(
                m_clock_setting_dialog->get_main_timer_form_setting()
                );
    ui->clock_alarmClock_form->set_default_alarm_clock(
                m_clock_setting_dialog->get_alarm_clock_default_setting()
                );

    save_data();
}



void Clock_Main_Window::systemTrayMenu_Button_show_or_hide_clicked()
{
    if(m_is_mainWindow_show == true)
    {
        mainWindow_hide();
    }
    else
    {
        mainWindow_show();
    }
}
void Clock_Main_Window::systemTrayMenu_Button_exit_clicked()
{
    QMainWindow::close();
}

void Clock_Main_Window::on_action_aboutSmallclock_triggered()
{
    Clock_About_Dialog about_dialog(this);
    about_dialog.exec();
}
