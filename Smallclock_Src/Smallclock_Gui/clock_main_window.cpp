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

#include "clock_main_window.h"
#include "./ui_clock_main_window.h"

#include "../../Smallclock_version.h"

using namespace std;
using namespace this_thread;

//usec
__int128 m_int128_stopwatch_time_use = 0;
__int128 m_int128_stopwatch_begin_time = 0;

__int128 m_int128_timer_duration = 0;
__int128 m_int128_timer_time_use = 0;
__int128 m_int128_timer_begin_time = 0;

Clock_Main_Window::alarmClock_set* m_alarm_clock_item_last_clicked = 0;

bool m_is_stopwatch_start = false;

bool m_is_clocktimer_start = false;
bool m_is_clocktimer_end = false;
bool m_is_clocktimer_reminding = false;

bool m_is_mainWindow_show = true;

const int m_const_int_flush_time = 53;

const string m_const_string_Smallclock_version =
        "<Smallclock_version=" +
        to_string(Smallcllock_version_major) +
        "." +
        to_string(Smallcllock_version_minor) +
        "." +
        to_string(Smallcllock_version_revision) +
        ">";

struct timeval tv_temp;

Command_Do_In_Thread<Clock_Main_Window> *m_command_thread_clocktimer = 0;

Clock_Main_Window::Clock_Main_Window(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::Clock_Main_Window)
{
    ui->setupUi(this);

    setWindowIcon(QIcon(":/program_icons/SmallClock.png"));
    
    m_timer_flush = new QTimer(this);
    connect(m_timer_flush, SIGNAL(timeout()), this, SLOT(do_when_flush_timer_timeout()));
    
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
                    SLOT(on_treeWidgetItem_rightKey_menu_clicked(const QPoint&)));
    
    m_menu_systemTray = new QMenu(this);

    m_action_menu_systemTray_show_hide = new QAction(m_menu_systemTray);
    m_action_menu_systemTray_show_hide->setText("隐藏");
    connect(m_action_menu_systemTray_show_hide, SIGNAL(triggered()), this,
            SLOT(on_systemTrayMenu_Button_show_or_hide_clicked()));

    m_action_menu_systemTray_exit = new QAction(m_menu_systemTray);
    m_action_menu_systemTray_exit->setText("退出");
    connect(m_action_menu_systemTray_exit, SIGNAL(triggered()), this,
            SLOT(on_systemTrayMenu_Button_exit_clicked()));

    m_menu_systemTray->addAction(m_action_menu_systemTray_show_hide);
    m_menu_systemTray->addAction(m_action_menu_systemTray_exit);

    m_system_tray = new QSystemTrayIcon(this);
    m_system_tray->setContextMenu(m_menu_systemTray);
    m_system_tray->setToolTip("SystemTray Program");
    m_system_tray->setIcon(QIcon(":/program_icons/SmallClock_SystemTray.png"));
    m_system_tray->show();

    clocktimer_initialization();
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

void Clock_Main_Window::qtimer_start()
{
    if(m_is_stopwatch_start == true || m_is_clocktimer_start == true)
    {
        m_timer_flush->start(m_const_int_flush_time);
    }
}
void Clock_Main_Window::qtimer_stop()
{
    if(m_is_stopwatch_start == false && m_is_clocktimer_start == false)
    {
        m_timer_flush->stop();
    }
}

void Clock_Main_Window::stopwatch_start()
{
    m_is_stopwatch_start = true;
    ui->pushButton_stopwatch_start_stop->setText("暂停");
    qtimer_start();
}
void Clock_Main_Window::stopwatch_stop()
{
    m_is_stopwatch_start = false;
    qtimer_stop();
    ui->pushButton_stopwatch_start_stop->setText("开始");
}
void Clock_Main_Window::stopwatch_flush()
{
    ui->clock_number_display_form_stopwatch->set_time(m_int128_stopwatch_time_use + (get_tv_time_now() - m_int128_stopwatch_begin_time));
}

void Clock_Main_Window::clocktimer_start()
{
    m_is_clocktimer_start = true;
    ui->pushButton_timer_start_stop->setText("暂停");
    qtimer_start();
}
void Clock_Main_Window::clocktimer_stop()
{
    m_is_clocktimer_start = false;
    qtimer_stop();
    ui->pushButton_timer_start_stop->setText("开始");
}
void Clock_Main_Window::clocktimer_end()
{
    m_is_clocktimer_end = true;
    ui->pushButton_timer_start_stop->setText("停止");
    ui->clock_number_display_form_timer->start_flickered_remind();
}
void Clock_Main_Window::clocktimer_flush()
{
    __int128 time_use = m_int128_timer_time_use + (get_tv_time_now() - m_int128_timer_begin_time);
    if(m_int128_timer_duration - time_use > 0)
    {
        ui->clock_number_display_form_timer->set_time(m_int128_timer_duration - time_use);
    }
    else
    {
        ui->clock_number_display_form_timer->set_time(time_use - m_int128_timer_duration);
        if(m_is_clocktimer_end == false)
        {
            clocktimer_end();
            do_when_clock_timer_end();
        }
    }
}

void Clock_Main_Window::do_when_clock_timer_end()
{
    m_is_clocktimer_reminding = true;

    Message_Window_Show::showMessageWindow("Small Clock",
                                           "计时器已到时",
                                           m_clock_setting_dialog->get_timer_message().c_str(),
                                           NULL);

    m_command_thread_clocktimer = new Command_Do_In_Thread<Clock_Main_Window>
            (m_clock_setting_dialog->get_timer_command(),
             this,
             &Clock_Main_Window::do_when_clocktimer_thread_exit,
             &Clock_Main_Window::do_when_clock_timer_stop_remind);
    
    cout << "ding----" << endl;
}
void Clock_Main_Window::do_when_clock_timer_stop_remind()
{
    m_is_clocktimer_reminding = false;
    cout << "turn off" << endl;
    ui->clock_number_display_form_timer->stop_flickered_remind();
    
}
void Clock_Main_Window::do_when_clocktimer_thread_exit()
{
    m_is_clocktimer_reminding = false;
    cout << "thread exit" << endl;
    ui->clock_number_display_form_timer->stop_flickered_remind();
    clocktimer_initialization();
}
void Clock_Main_Window::clocktimer_initialization()
{
    m_int128_timer_duration = 0;
    m_int128_timer_begin_time = 0;
    m_int128_timer_time_use = 0;
    m_command_thread_clocktimer = 0;
    m_is_clocktimer_start = false;
    m_is_clocktimer_end = false;
    m_is_clocktimer_reminding = true;
    ui->clock_number_display_form_timer->set_time(0);
    ui->pushButton_timer_start_stop->setText("开始");
    ui->pushButton_timer_start_stop->setDisabled(true);
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
    if(event->spontaneous() == true) //事件来自底层窗口系统
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
        else
        {
            save_data();
        }
    }
}

string read_str(ifstream* readFile)
{
#define CHAR_BUF_SIZE 16
    string data;
    char buf[CHAR_BUF_SIZE + 1];
    memset(buf, 0, sizeof(buf));
    int read_data = 0;
    for(int i = 0; i < CHAR_BUF_SIZE; i++)
    {
        read_data = readFile->get();
        if(read_data == 0 || read_data == -1)
        {
            string bufData(buf);
            data += bufData;

            break;
        }
        else
        {
            buf[i] = read_data;
        }
        if(i >= CHAR_BUF_SIZE - 1)
        {
            i = -1;
            string bufData(buf);
            data += bufData;

            memset(buf, 0, sizeof(buf));
        }
    }
    return data;

#undef INT_BUF_SIZE
}
void write_str(ofstream* saveFile, string str)
{
    char char_zero = 0;
    saveFile->write((char*)(str.c_str()), sizeof(char) * str.size());
    cout << sizeof(char_zero) << endl;
    saveFile->write(&char_zero, sizeof(char_zero));
}
void save_alarmClock(ofstream* saveFile, Alarm_Clock alarm_clock)
{
    write_str(saveFile, alarm_clock.m_str_alarm_clock_name);
    write_str(saveFile, alarm_clock.m_str_message);
    write_str(saveFile, alarm_clock.m_str_command);

    saveFile->write((char*)(&alarm_clock.m_timeType_choose_range),
                    sizeof(alarm_clock.m_timeType_choose_range));

    saveFile->write((char*)(&alarm_clock.m_longlongint_accurate_time),
                    sizeof(alarm_clock.m_longlongint_accurate_time));
    saveFile->write((char*)(&alarm_clock.m_month), sizeof(alarm_clock.m_month));
    saveFile->write((char*)(&alarm_clock.m_day), sizeof(alarm_clock.m_day));
    saveFile->write((char*)(&alarm_clock.m_hour), sizeof(alarm_clock.m_hour));
    saveFile->write((char*)(&alarm_clock.m_min), sizeof(alarm_clock.m_min));

    saveFile->write((char*)(&alarm_clock.m_is_turn_on), sizeof(alarm_clock.m_is_turn_on));

    int day_in_week_length = alarm_clock.m_vector_alarm_time_range_a_week.size();
    saveFile->write((char*)(&day_in_week_length), sizeof(day_in_week_length));
    for(int i = 0; i < day_in_week_length; i++)
    {
        saveFile->write((char*)(&alarm_clock.m_vector_alarm_time_range_a_week[i]),
                        sizeof(alarm_clock.m_vector_alarm_time_range_a_week[i]));
    }
}
Alarm_Clock read_alarmClock(ifstream* loadFile)
{
    Alarm_Clock alarm_clock;
    alarm_clock.m_str_alarm_clock_name = read_str(loadFile);
    alarm_clock.m_str_message = read_str(loadFile);
    alarm_clock.m_str_command = read_str(loadFile);

    loadFile->read((char*)(&alarm_clock.m_timeType_choose_range),
                   sizeof(alarm_clock.m_timeType_choose_range));

    loadFile->read((char*)(&alarm_clock.m_longlongint_accurate_time),
                   sizeof(alarm_clock.m_longlongint_accurate_time));
    loadFile->read((char*)(&alarm_clock.m_month), sizeof(alarm_clock.m_month));
    loadFile->read((char*)(&alarm_clock.m_day), sizeof(alarm_clock.m_day));
    loadFile->read((char*)(&alarm_clock.m_hour), sizeof(alarm_clock.m_hour));
    loadFile->read((char*)(&alarm_clock.m_min), sizeof(alarm_clock.m_min));

    loadFile->read((char*)(&alarm_clock.m_is_turn_on), sizeof(alarm_clock.m_is_turn_on));

    int day_in_week_length = 0;
    loadFile->read((char*)(&day_in_week_length), sizeof(day_in_week_length));
    Alarm_Clock::Day_In_Week temp_read;
    for(int i = 0; i < day_in_week_length; i++)
    {
        loadFile->read((char*)(&temp_read), sizeof(temp_read));
        alarm_clock.m_vector_alarm_time_range_a_week.push_back(temp_read);
    }
    return alarm_clock;
}
void Clock_Main_Window::save_data()
{
    ofstream save_file("Smallclock_Data");

    write_str(&save_file, m_const_string_Smallclock_version);

    write_str(&save_file, m_clock_setting_dialog->get_timer_command());
    write_str(&save_file, m_clock_setting_dialog->get_timer_message());

    bool is_sendTips_when_window_will_be_close =
            m_clock_setting_dialog->get_is_sendTips_when_window_will_be_close();
    save_file.write((char*)(&is_sendTips_when_window_will_be_close),
                    sizeof(is_sendTips_when_window_will_be_close));
    bool is_hide_when_mainWindow_clockButton_click =
            m_clock_setting_dialog->get_is_hide_when_mainWindow_clockButton_click();
    save_file.write((char*)(&is_hide_when_mainWindow_clockButton_click),
                    sizeof(is_hide_when_mainWindow_clockButton_click));
    bool is_hide_when_app_start =
            m_clock_setting_dialog->get_is_hide_when_app_start();
    save_file.write((char*)(&is_hide_when_app_start),
                    sizeof(is_hide_when_app_start));

    save_alarmClock(&save_file, m_clock_setting_dialog->get_alarm_default_setting());

    int num_of_alarmClock = 0;
    QTreeWidgetItemIterator itor_get_alarm_clock_num(ui->treeWidget_alarm_clock);
    while(*itor_get_alarm_clock_num)
    {
        num_of_alarmClock++;

        (*itor_get_alarm_clock_num)->text(0);
        ++itor_get_alarm_clock_num;
    }
    save_file.write((char*)(&num_of_alarmClock), sizeof(num_of_alarmClock));

    QTreeWidgetItemIterator itor_write_alarm_clock(ui->treeWidget_alarm_clock);
    while(*itor_write_alarm_clock)
    {
        alarmClock_set* alarm_clock_set = (alarmClock_set*)(*itor_write_alarm_clock);
        save_alarmClock(&save_file, alarm_clock_set->get_alarmClock());

        (*itor_write_alarm_clock)->text(0);
        ++itor_write_alarm_clock;
    }
}
void Clock_Main_Window::read_data()
{
    ifstream load_file("Smallclock_Data");

    string smallclock_version = read_str(&load_file);
    if(smallclock_version == m_const_string_Smallclock_version)
    {
        m_clock_setting_dialog->set_timer_command(read_str(&load_file));
        m_clock_setting_dialog->set_timer_message(read_str(&load_file));

        bool is_sendTips_when_window_will_be_close = 0;
        bool is_hide_when_mainWindow_clockButton_click = 0;
        bool is_hide_when_app_start = 0;
        load_file.read((char*)(&is_sendTips_when_window_will_be_close),
                       sizeof(is_sendTips_when_window_will_be_close));
        load_file.read((char*)(&is_hide_when_mainWindow_clockButton_click),
                       sizeof(is_hide_when_mainWindow_clockButton_click));
        load_file.read((char*)(&is_hide_when_app_start),
                       sizeof(is_hide_when_app_start));
        m_clock_setting_dialog
                ->set_is_sendTips_when_window_will_be_close(is_sendTips_when_window_will_be_close);
        m_clock_setting_dialog
                ->set_is_hide_when_mainWindow_clockButton_click(is_hide_when_mainWindow_clockButton_click);
        m_clock_setting_dialog->set_is_hide_when_app_start(is_hide_when_app_start);

        m_clock_setting_dialog->set_alarm_default_setting(read_alarmClock(&load_file));

        int num_of_alarm_clock = 0;
        load_file.read((char*)(&num_of_alarm_clock), sizeof(num_of_alarm_clock));

        for(int i = 0; i < num_of_alarm_clock; i++)
        {
            Alarm_Clock alarm_clock_load = read_alarmClock(&load_file);
            alarmClock_add(alarm_clock_load);
        }
    }
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
    QDateTime alarm_clock_date_time = QDateTime::fromTime_t(alarm_clock.m_longlongint_accurate_time);
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

void Clock_Main_Window::do_when_flush_timer_timeout()
{
    if(m_is_stopwatch_start == true)
    {
        stopwatch_flush();
    }
    if(m_is_clocktimer_start == true)
    {
        clocktimer_flush();
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

void Clock_Main_Window::on_treeWidgetItem_rightKey_menu_clicked(const QPoint& point)
{
    m_alarm_clock_item_last_clicked = (alarmClock_set*)(ui->treeWidget_alarm_clock->itemAt(point));
    QMenu* menu_treeWidget = new QMenu();
    if(m_alarm_clock_item_last_clicked == 0)
    {
        menu_treeWidget->addAction("新建闹钟", this, SLOT(on_Button_alarmClock_add_new_clicked()));
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
        menu_treeWidget->addAction("编辑闹钟", this, SLOT(on_Button_alarmClock_edit_clicked()));
        menu_treeWidget->addAction("删除闹钟", this, SLOT(on_Button_alarmClock_delete_clicked()));
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

void Clock_Main_Window::on_pushButton_stopwatch_start_stop_clicked()
{
    if(m_is_stopwatch_start == false)
    {
        m_int128_stopwatch_begin_time = get_tv_time_now();
        
        ui->clock_number_display_form_stopwatch
                ->set_time(m_int128_stopwatch_time_use + (get_tv_time_now() - m_int128_stopwatch_begin_time));
        stopwatch_start();
    }
    else
    {
        stopwatch_stop();
        
        long end_time = get_tv_time_now();
        
        ui->clock_number_display_form_stopwatch
                ->set_time(m_int128_stopwatch_time_use + (end_time - m_int128_stopwatch_begin_time));
        
        m_int128_stopwatch_time_use += (end_time - m_int128_stopwatch_begin_time);
        m_int128_stopwatch_begin_time = 0;
    }
}

void Clock_Main_Window::on_pushButton_stopwatch_reset_clicked()
{
    stopwatch_stop();
    m_int128_stopwatch_time_use = 0;
    ui->clock_number_display_form_stopwatch->set_time(0);
}

void Clock_Main_Window::on_pushButton_timer_set_clicked()
{
    ClockTimer_Choose_Time_Dialog *choose_time_dialog = new ClockTimer_Choose_Time_Dialog(this);
    //0 -> 取消 1 -> 确认
    int click_button = choose_time_dialog->exec();
    if(click_button == 1)
    {
        m_int128_timer_duration = choose_time_dialog->get_hour() * (60 * 60) * (1000 * 1000) +
                choose_time_dialog->get_min() * 60 * (1000 * 1000) +
                choose_time_dialog->get_sec() * (1000 * 1000);
        ui->clock_number_display_form_timer->set_time(m_int128_timer_duration);
        ui->pushButton_timer_start_stop->setDisabled(false);
    }
}

void Clock_Main_Window::on_pushButton_timer_start_stop_clicked()
{
    if(m_is_clocktimer_start == false)
    {
        m_int128_timer_begin_time = get_tv_time_now();
        m_int128_timer_time_use += (get_tv_time_now() - m_int128_timer_begin_time);
        
        ui->clock_number_display_form_timer
                ->set_time(m_int128_timer_duration - m_int128_timer_time_use);
        
        clocktimer_start();
    }
    else
    {
        if(m_is_clocktimer_end == false)
        {
            clocktimer_stop();
            
            m_int128_timer_time_use += (get_tv_time_now() - m_int128_timer_begin_time);
            ui->clock_number_display_form_timer
                    ->set_time(m_int128_timer_duration - m_int128_timer_time_use);
            
            m_int128_timer_begin_time = 0;
        }
        else
        {
            m_command_thread_clocktimer->end_the_command();
            ui->clock_number_display_form_timer->stop_flickered_remind();
            clocktimer_initialization();
        }
    }
}

void Clock_Main_Window::on_pushButton_timer_reset_clicked()
{
    if(m_is_clocktimer_reminding == true)
    {
        do_when_clock_timer_stop_remind();
    }
    clocktimer_initialization();
}

void Clock_Main_Window::on_action_set_triggered()
{
    m_clock_setting_dialog->exec();
}

void Clock_Main_Window::on_Button_alarmClock_add_new_clicked()
{
    New_Alarm_Clock_Dialog *new_alarm_clock_dialog = new New_Alarm_Clock_Dialog();
    new_alarm_clock_dialog->set_alarm_clock(m_clock_setting_dialog->get_alarm_default_setting());
    if(new_alarm_clock_dialog->exec() == 1)
    {
        alarmClock_add(new_alarm_clock_dialog->get_alarm_clock_setting());
    }
}
void Clock_Main_Window::on_Button_alarmClock_edit_clicked()
{
    New_Alarm_Clock_Dialog *new_alarm_clock_dialog = new New_Alarm_Clock_Dialog();
    new_alarm_clock_dialog->set_alarm_clock(m_alarm_clock_item_last_clicked->get_alarmClock());
    if(new_alarm_clock_dialog->exec() == 1)
    {
        alarmClock_edit(new_alarm_clock_dialog->get_alarm_clock_setting(),
                        m_alarm_clock_item_last_clicked);
    }
}
void Clock_Main_Window::on_Button_alarmClock_delete_clicked()
{
    m_alarm_clock_item_last_clicked->stop_remind();
    alarmClock_delete(m_alarm_clock_item_last_clicked);
}

void Clock_Main_Window::on_systemTrayMenu_Button_show_or_hide_clicked()
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
void Clock_Main_Window::on_systemTrayMenu_Button_exit_clicked()
{
    QMainWindow::close();
}
