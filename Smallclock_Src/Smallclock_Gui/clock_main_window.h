#ifndef CLOCK_MAIN_WINDOW_H
#define CLOCK_MAIN_WINDOW_H

#include <QMainWindow>
#include <QSystemTrayIcon>
#include <QTreeWidgetItem>
#include <QTimer>
#include <QCloseEvent>

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

    class alarmClock_set : public QTreeWidgetItem
    {
    public:
        alarmClock_set(Alarm_Clock alarm_clock)
        {
            QTreeWidgetItem(6);
            edit_alarm_clock(alarm_clock);
        }

        Alarm_Clock get_alarmClock()
        {
            return m_alarm_clock;
        }

        bool get_alarm_clock_is_turn()
        {
            return m_alarm_clock.m_is_turn_on;
        }
        bool get_alarm_clock_is_remind()
        {
            return m_alarm_clock.m_is_remind;
        }

        void edit_alarm_clock(Alarm_Clock alarm_clock)
        {
            m_alarm_clock = alarm_clock;
            m_longlongint_secsSince1Jan1970UTC_last_remind = 0;
            show_alarmClock_date();
        }

        void start_remind()
        {
            //上次开始提醒在1分钟以前
            if(((m_longlongint_secsSince1Jan1970UTC_last_remind / 60) * 60) + 60 <
                    QDateTime::currentDateTime().toSecsSinceEpoch())
            {
                m_alarm_clock.m_is_remind = true;
                m_longlongint_secsSince1Jan1970UTC_last_remind =
                        QDateTime::currentDateTime().toSecsSinceEpoch();
                m_thread_do_command
                        = new Command_Do_In_Thread<alarmClock_set>(m_alarm_clock.m_str_command,
                                                                   this,
                                                                   &alarmClock_set::finish_remind);

                send_message();
                show_alarmClock_date();
            }
        }
        void stop_remind()
        {
            if(m_thread_do_command != 0)
            {
                m_thread_do_command->end_the_command();
            }

            finish_remind();
        }
    private:
        Alarm_Clock m_alarm_clock;
        Command_Do_In_Thread<alarmClock_set> *m_thread_do_command = 0;
        long long int m_longlongint_secsSince1Jan1970UTC_last_remind = 0;

        void show_alarmClock_date()
        {
            //is turn on
            this->setText(5, QString(m_alarm_clock.get_str_alarm_clock_state().c_str()));
            //command
            this->setText(4, QString(m_alarm_clock.m_str_command.c_str()));
            //message
            this->setText(3, QString(m_alarm_clock.m_str_message.c_str()));
            //次数
            this->setText(2, QString(m_alarm_clock.get_str_timeType_choose_range().c_str()));
            //time
            this->setText(1, QString(m_alarm_clock.get_str_time().c_str()));
            //name
            this->setText(0, QString(m_alarm_clock.m_str_alarm_clock_name.c_str()));

        }

        void finish_remind()
        {
            m_thread_do_command = 0;
            m_alarm_clock.m_is_remind = false;
            show_alarmClock_date();
        }

        void send_message()
        {
            Message_Window_Show::showMessageWindow("Small Clock",
                                                   "闹钟已到时",
                                                   m_alarm_clock.m_str_message.c_str(),
                                                   NULL);
        }
    };

private slots:
    void do_when_flush_timer_timeout();

    void do_when_alarm_clock_timer_timeout();

    void do_when_alarmClock_to_be_stop_remind();
    void do_when_alarmClock_to_be_start_or_stop();
    void alarmClockTreeWidgetItem_rightKey_menu_clicked(const QPoint& point);

    void on_pushButton_stopwatch_start_stop_clicked();

    void on_pushButton_stopwatch_reset_clicked();

    void on_pushButton_timer_set_clicked();

    void on_pushButton_timer_start_stop_clicked();

    void on_pushButton_timer_reset_clicked();

    void on_action_set_triggered();

    void pushButton_alarmClock_add_new_clicked();
    void pushButton_alarmClock_edit_clicked();
    void pushButton_alarmClock_delete_clicked();

    void systemTrayMenu_Button_show_or_hide_clicked();
    void systemTrayMenu_Button_exit_clicked();

private:
    Ui::Clock_Main_Window *ui;

    QTimer *m_timer_flush;
    QTimer *m_timer_alarm_clock;

    Clock_Setting_Dialog *m_clock_setting_dialog;

    QSystemTrayIcon* m_system_tray;
    QMenu* m_menu_systemTray;
    QAction* m_action_menu_systemTray_show_hide;
    QAction* m_action_menu_systemTray_exit;

    __int128 get_tv_time_now();

    void qtimer_start();
    void qtimer_stop();

    void stopwatch_start();
    void stopwatch_stop();
    void stopwatch_flush();

    void clocktimer_start();
    void clocktimer_stop();
    void clocktimer_end();
    void clocktimer_flush();
    void do_when_clock_timer_end();
    void do_when_clock_timer_stop_remind();
    void do_when_clocktimer_thread_exit();
    void clocktimer_initialization();

    bool compare_alarm_clock_day_time_and_now(Alarm_Clock alarm_clock);
    bool compare_alarm_clock_week_day_and_now(Alarm_Clock alarm_clock);
    bool compare_alarm_clock_month_and_now(Alarm_Clock alarm_clock);
    bool compare_alarm_clock_dateTime_and_now(Alarm_Clock alarm_clock);

    void alarmClock_add(Alarm_Clock new_alarm_clock);
    void alarmClock_edit(Alarm_Clock new_alarm_clock, alarmClock_set *alarm_clock_set);
    void alarmClock_delete(alarmClock_set *alarm_clock_set);

    void mainWindow_show();
    void mainWindow_hide();

    void closeEvent(QCloseEvent *event);

    void save_data();
    void read_data();
};
#endif // CLOCK_MAIN_WINDOW_H
