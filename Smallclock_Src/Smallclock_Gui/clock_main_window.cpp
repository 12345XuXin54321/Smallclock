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

Clock_Main_Window::alarmClock_set* m_alarm_clock_item_last_clicked = 0;

bool m_is_stopwatch_start = false;

bool m_is_mainWindow_show = true;

const string m_const_string_Smallclock_version =
        "<Smallclock_version=" +
        to_string(APP_VERSION_MAJOR) +
        "." +
        to_string(APP_VERSION_MINOR) +
        "." +
        to_string(APP_VERSION_REVISION) +
        ">";

struct timeval tv_temp;

Clock_Main_Window::Clock_Main_Window(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::Clock_Main_Window)
{
    ui->setupUi(this);

    setWindowIcon(QIcon(":/program_icons/SmallClock.png"));

    m_timer_alarm_clock = new QTimer(this);
    connect(m_timer_alarm_clock, SIGNAL(timeout()), this, SLOT(do_when_alarm_clock_timer_timeout()));
    m_timer_alarm_clock->start(800);

    m_clock_setting_dialog = new Clock_Setting_Dialog();
    m_clock_setting_dialog->hide();

    ui->treeWidget_alarm_clock->setContextMenuPolicy(Qt::ContextMenuPolicy::CustomContextMenu);
    ui->treeWidget_alarm_clock->
            connect(ui->treeWidget_alarm_clock,
                    SIGNAL(customContextMenuRequested(const QPoint&)),
                    this,
                    SLOT(alarmClockTreeWidgetItem_rightKey_menu_clicked(const QPoint&)));

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

__int128 Clock_Main_Window::get_tv_time_now()
{
    gettimeofday(&tv_temp, NULL);
    return (tv_temp.tv_sec * 1000000 + tv_temp.tv_usec);
}


void Clock_Main_Window::alarmClock_add(Alarm_Clock new_alarm_clock)
{
    alarmClock_set* alarm_clock_set = new alarmClock_set(new_alarm_clock);
    ui->treeWidget_alarm_clock->addTopLevelItem(alarm_clock_set);
}
void Clock_Main_Window::alarmClock_edit(Alarm_Clock new_alarm_clock,
                                        alarmClock_set* alarm_clock_set)
{
    alarm_clock_set->edit_alarm_clock(new_alarm_clock);
}
void Clock_Main_Window::alarmClock_delete(alarmClock_set *alarm_clock_set)
{
    int index = ui->treeWidget_alarm_clock->
            indexOfTopLevelItem(alarm_clock_set);
    ui->treeWidget_alarm_clock->takeTopLevelItem(index);
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

    //save_alarmClock(&m_save_data, m_clock_setting_dialog->get_alarm_default_setting());
    m_clock_setting_dialog->get_alarm_default_setting().write_save_data(&m_save_data);

    int num_of_alarmClock = 0;
    QTreeWidgetItemIterator itor_get_alarm_clock_num(ui->treeWidget_alarm_clock);
    while(*itor_get_alarm_clock_num)
    {
        num_of_alarmClock++;

        (*itor_get_alarm_clock_num)->text(0);
        ++itor_get_alarm_clock_num;
    }
    m_save_data.write_next_data<int>(num_of_alarmClock);

    QTreeWidgetItemIterator itor_write_alarm_clock(ui->treeWidget_alarm_clock);
    while(*itor_write_alarm_clock)
    {
        alarmClock_set* alarm_clock_set = (alarmClock_set*)(*itor_write_alarm_clock);
        //save_alarmClock(&m_save_data, alarm_clock_set->get_alarmClock());
        alarm_clock_set->get_alarmClock().write_save_data(&m_save_data);

        (*itor_write_alarm_clock)->text(0);
        ++itor_write_alarm_clock;
    }

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

        //m_clock_setting_dialog->set_alarm_default_setting(read_alarmClock(&m_save_data));
        Alarm_Clock default_alarmClock;
        default_alarmClock.load_save_data(&m_save_data);
        m_clock_setting_dialog->set_alarm_default_setting(default_alarmClock);

        int num_of_alarm_clock = m_save_data.read_next_data<int>();

        for(int i = 0; i < num_of_alarm_clock; i++)
        {
            Alarm_Clock alarm_clock_load;// = read_alarmClock(&m_save_data);
            alarm_clock_load.load_save_data(&m_save_data);
            alarmClock_add(alarm_clock_load);
        }
    }

    m_save_data.finish_read();
}

bool Clock_Main_Window::compare_alarm_clock_day_time_and_now(Alarm_Clock alarm_clock)
{
    QTime now_time = QTime::currentTime();
    if(now_time.hour() == alarm_clock.m_hour &&
            now_time.minute() == alarm_clock.m_min)
    {
        return true;
    }
    else
    {
        return false;
    }
}
bool Clock_Main_Window::compare_alarm_clock_week_day_and_now(Alarm_Clock alarm_clock)
{
    QDateTime now_date_time = QDateTime::currentDateTime();
    cout << now_date_time.date().dayOfWeek() << endl;
    for(auto day_in_week : alarm_clock.m_vector_alarm_time_range_a_week)
    {
        if(now_date_time.date().dayOfWeek() == day_in_week)
        {
            return true;
        }
    }
    return false;
}
bool Clock_Main_Window::compare_alarm_clock_month_and_now(Alarm_Clock alarm_clock)
{
    QDate now_date = QDate::currentDate();
    if(now_date.month() == alarm_clock.m_month)
    {
        return true;
    }
    else
    {
        return false;
    }
}
bool Clock_Main_Window::compare_alarm_clock_dateTime_and_now(Alarm_Clock alarm_clock)
{
    QDateTime now_date_time = QDateTime::currentDateTime();
    QDateTime alarm_clock_date_time = QDateTime::fromSecsSinceEpoch(alarm_clock.m_longlongint_accurate_time);
    if(now_date_time.date().year() == alarm_clock_date_time.date().year() &&
            now_date_time.date().dayOfYear() == alarm_clock_date_time.date().dayOfYear() &&
            now_date_time.time().hour() == alarm_clock_date_time.time().hour() &&
            now_date_time.time().minute() == alarm_clock_date_time.time().minute())
    {
        return true;
    }
    else
    {
        return false;
    }
}


void Clock_Main_Window::do_when_alarm_clock_timer_timeout()
{
    QTreeWidgetItemIterator itor(ui->treeWidget_alarm_clock);
    while(*itor)
    {
        alarmClock_set* alarm_clock_set = (alarmClock_set*)(*itor);
        Alarm_Clock alarm_clock = alarm_clock_set->get_alarmClock();

        if(alarm_clock.m_is_turn_on == true &&
                alarm_clock.m_is_remind == false)
        {
            switch(alarm_clock.m_timeType_choose_range)
            {
                case Alarm_Clock::Time_Type::range_a_day:
                    {
                        if(compare_alarm_clock_day_time_and_now(alarm_clock) == true)
                        {
                            alarm_clock_set->start_remind();
                        }
                        break;
                    }
                case Alarm_Clock::Time_Type::range_a_week:
                    {
                        if(compare_alarm_clock_day_time_and_now(alarm_clock) == true &&
                                compare_alarm_clock_week_day_and_now(alarm_clock) == true)
                        {
                            alarm_clock_set->start_remind();
                        }
                        break;
                    }
                case Alarm_Clock::Time_Type::range_a_year:
                    {
                        if(compare_alarm_clock_day_time_and_now(alarm_clock) == true &&
                                compare_alarm_clock_month_and_now(alarm_clock) == true)
                        {
                            alarm_clock_set->start_remind();
                        }
                        break;
                    }
                case Alarm_Clock::Time_Type::no_range:
                    {
                        if(compare_alarm_clock_dateTime_and_now(alarm_clock) == true)
                        {
                            alarm_clock_set->start_remind();
                        }
                        break;
                    }
            }
        }

        (*itor)->text(0);
        ++itor;
    }
}

void Clock_Main_Window::alarmClockTreeWidgetItem_rightKey_menu_clicked(const QPoint& point)
{
    m_alarm_clock_item_last_clicked = (alarmClock_set*)(ui->treeWidget_alarm_clock->itemAt(point));
    QMenu* menu_treeWidget = new QMenu();
    if(m_alarm_clock_item_last_clicked == 0)
    {
        menu_treeWidget->addAction("新建闹钟", this, SLOT(pushButton_alarmClock_add_new_clicked()));
    }
    else
    {
        if(m_alarm_clock_item_last_clicked->get_alarmClock().m_is_turn_on == true)
        {
            if(m_alarm_clock_item_last_clicked->get_alarmClock().m_is_remind == true)
            {
                menu_treeWidget->addAction("停止提醒", this, SLOT(do_when_alarmClock_to_be_stop_remind()));
            }
            else
            {
                menu_treeWidget->addAction("关闭", this, SLOT(do_when_alarmClock_to_be_start_or_stop()));
            }
        }
        else
        {
            menu_treeWidget->addAction("开启", this, SLOT(do_when_alarmClock_to_be_start_or_stop()));
        }
        menu_treeWidget->addSeparator();
        menu_treeWidget->addAction("编辑闹钟", this, SLOT(pushButton_alarmClock_edit_clicked()));
        menu_treeWidget->addAction("删除闹钟", this, SLOT(pushButton_alarmClock_delete_clicked()));
    }
    menu_treeWidget->exec(QCursor::pos());
}

void Clock_Main_Window::do_when_alarmClock_to_be_stop_remind()
{
    m_alarm_clock_item_last_clicked->stop_remind();
}
void Clock_Main_Window::do_when_alarmClock_to_be_start_or_stop()
{
    Alarm_Clock alarm_clock = m_alarm_clock_item_last_clicked->get_alarmClock();
    alarm_clock.m_is_turn_on ^= 1;
    alarm_clock.m_is_remind = false;
    m_alarm_clock_item_last_clicked->edit_alarm_clock(alarm_clock);
}

void Clock_Main_Window::on_action_set_triggered()
{
    m_clock_setting_dialog->show_main_timer_form_setting(
                ui->clock_timer_form->get_data()
                );

    m_clock_setting_dialog->exec();

    ui->clock_timer_form->set_data(
                m_clock_setting_dialog->get_main_timer_form_setting()
                );

    save_data();
}

void Clock_Main_Window::pushButton_alarmClock_add_new_clicked()
{
    New_Alarm_Clock_Dialog *new_alarm_clock_dialog = new New_Alarm_Clock_Dialog();
    new_alarm_clock_dialog->set_alarm_clock(m_clock_setting_dialog->get_alarm_default_setting());
    if(new_alarm_clock_dialog->exec() == 1)
    {
        alarmClock_add(new_alarm_clock_dialog->get_alarm_clock_setting());
    }
}
void Clock_Main_Window::pushButton_alarmClock_edit_clicked()
{
    New_Alarm_Clock_Dialog *new_alarm_clock_dialog = new New_Alarm_Clock_Dialog();
    new_alarm_clock_dialog->set_alarm_clock(m_alarm_clock_item_last_clicked->get_alarmClock());
    if(new_alarm_clock_dialog->exec() == 1)
    {
        alarmClock_edit(new_alarm_clock_dialog->get_alarm_clock_setting(),
                        m_alarm_clock_item_last_clicked);
    }
}
void Clock_Main_Window::pushButton_alarmClock_delete_clicked()
{
    m_alarm_clock_item_last_clicked->stop_remind();
    alarmClock_delete(m_alarm_clock_item_last_clicked);
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
