#ifndef CLOCK_MAIN_ALARMCLOCK_FORM_H
#define CLOCK_MAIN_ALARMCLOCK_FORM_H

#include <list>

#include <QWidget>
#include <QTreeWidgetItem>
#include <QMenu>
#include <QTimer>

#include "../Smallclock_Base/App_Data_Save.h"
#include "../Smallclock_Base/Get_Time.h"
#include "../Smallclock_Base/Music_Player.h"

#include "new_alarm_clock_dialog.h"
#include "Message_Window_Show.h"

namespace Ui {
    class Clock_Main_AlarmClock_Form;
}

class Clock_Main_AlarmClock_Form : public QWidget
{
    Q_OBJECT

public:
    explicit Clock_Main_AlarmClock_Form(QWidget *parent = nullptr);
    ~Clock_Main_AlarmClock_Form();

    Alarm_Clock get_default_alarm_clock()
    {
        return m_alarm_clock_default_setting;
    }
    void set_default_alarm_clock(Alarm_Clock default_alarm_clock)
    {
        m_alarm_clock_default_setting = default_alarm_clock;
    }

    struct AlarmClock_Form_Data
    {
        Alarm_Clock alarm_clock_default_setting;
        list<Alarm_Clock> list_alarm_clock_using;
        void write_save_data(App_Data_Save* data_save)
        {
            alarm_clock_default_setting.m_alarm_clock_data.write_save_data(data_save);
            data_save->write_next_data<size_t>(list_alarm_clock_using.size());
            for(auto alarmClock : list_alarm_clock_using)
            {
                alarmClock.m_alarm_clock_data.write_save_data(data_save);
            }
        }
        void load_save_data(App_Data_Save* data_save)
        {
            list_alarm_clock_using.clear();

            alarm_clock_default_setting.m_alarm_clock_data.load_save_data(data_save);
            size_t num_of_using_alarm_clock = data_save->read_next_data<size_t>();
            for (size_t i = 0; i < num_of_using_alarm_clock; i++)
            {
                Alarm_Clock alarmClock;
                alarmClock.m_alarm_clock_data.load_save_data(data_save);
                list_alarm_clock_using.push_back(alarmClock);
            }
        }
    };

    void set_data(AlarmClock_Form_Data data);

    AlarmClock_Form_Data get_data();

private slots:
    void pushButton_alarmClock_add_new_clicked();
    void pushButton_alarmClock_edit_clicked();
    void pushButton_alarmClock_delete_clicked();

    void do_when_alarm_clock_timer_timeout();

    void do_when_alarmClock_to_be_stop_remind();
    void do_when_alarmClock_to_be_start_or_stop();
    void alarmClockTreeWidgetItem_rightKey_menu_clicked(const QPoint& point);

private:
    Ui::Clock_Main_AlarmClock_Form *ui;

    QTimer *m_timer_alarm_clock;

    Alarm_Clock m_alarm_clock_default_setting;

    class alarmClock_set : public QTreeWidgetItem
    {
    public:
        enum type_of_item
        {
            name_of_alarm_clock = 0,
            time_of_start = 1,
            type_of_time = 2,
            messages_that_send = 3,
            path_of_command = 4,
            path_of_music = 5,
            is_turn_on = 6
        };

        alarmClock_set(Alarm_Clock alarm_clock)
        {
            QTreeWidgetItem(7);
            edit_alarm_clock(alarm_clock);
        }

        Alarm_Clock get_alarmClock()
        {
            return m_alarm_clock;
        }

        bool get_alarm_clock_is_turn()
        {
            return m_alarm_clock.m_alarm_clock_data.is_turn_on;
        }
        bool get_alarm_clock_is_remind()
        {
            return m_music_player.is_media_player_playing();
        }

        void edit_alarm_clock(Alarm_Clock alarm_clock)
        {
            m_alarm_clock = alarm_clock;
            m_longlongint_secsSince1Jan1970UTC_last_remind = 0;
            stop_remind();
        }

        void start_remind()
        {
            //上次开始提醒在1分钟以前
            if(((m_longlongint_secsSince1Jan1970UTC_last_remind / 60) * 60) + 60 <
                    QDateTime::currentDateTime().toSecsSinceEpoch())
            {
                m_longlongint_secsSince1Jan1970UTC_last_remind =
                        QDateTime::currentDateTime().toSecsSinceEpoch();
                if(m_alarm_clock.m_alarm_clock_data.str_command_path.size() > 0)
                {
                    system((m_alarm_clock.m_alarm_clock_data.str_command_path + " &").c_str());
                }
                if(m_alarm_clock.m_alarm_clock_data.str_music_path.size() > 0)
                {
                    m_music_player.start_play(m_alarm_clock.m_alarm_clock_data.str_music_path);
                }

                send_message();
                show_alarmClock_date();
            }

        }
        void stop_remind()
        {
            m_music_player.stop_play();
            show_alarmClock_date();
        }
        void updata_alarmClock_date()
        {
            show_alarmClock_date();
        }

    private:
        Alarm_Clock m_alarm_clock;
        long long int m_longlongint_secsSince1Jan1970UTC_last_remind = 0;
        Music_Player m_music_player;

        void show_alarmClock_date()
        {
            //is turn on
            this->setText(is_turn_on,
                          QString(get_str_alarm_clock_state().c_str()));
            //music
            this->setText(path_of_music,
                          QString(m_alarm_clock.m_alarm_clock_data.str_music_path.c_str()));
            //command
            this->setText(path_of_command,
                          QString(m_alarm_clock.m_alarm_clock_data.str_command_path.c_str()));
            //message
            this->setText(messages_that_send,
                          QString(m_alarm_clock.m_alarm_clock_data.str_message.c_str()));
            //次数
            this->setText(type_of_time,
                          QString(m_alarm_clock.get_str_timeType_choose_range().c_str()));
            //time
            this->setText(time_of_start,
                          QString(m_alarm_clock.get_str_time().c_str()));
            //name
            this->setText(name_of_alarm_clock,
                          QString(m_alarm_clock.m_alarm_clock_data.str_alarm_clock_name.c_str()));

        }

        void send_message()
        {
            Message_Window_Show::showMessageWindow(
                        "闹钟已到时",
                        m_alarm_clock.m_alarm_clock_data.str_message.c_str()
                        );
        }

        string get_str_alarm_clock_state()
        {
            if(m_alarm_clock.m_alarm_clock_data.is_turn_on == false)
            {
                return "未开启";
            }
            else
            {
                if(m_music_player.is_media_player_playing() == true)
                {
                    return "提醒中";
                }
                else
                {
                    return "已开启";
                }
            }
        }
    };

    alarmClock_set* m_alarm_clock_item_last_clicked = 0;

    bool compare_alarm_clock_day_time_and_now(Alarm_Clock alarm_clock);
    bool compare_alarm_clock_week_day_and_now(Alarm_Clock alarm_clock);
    bool compare_alarm_clock_month_and_now(Alarm_Clock alarm_clock);
    bool compare_alarm_clock_dateTime_and_now(Alarm_Clock alarm_clock);

    void alarmClock_add(Alarm_Clock new_alarm_clock);
    void alarmClock_edit(Alarm_Clock new_alarm_clock, alarmClock_set *alarm_clock_set);
    void alarmClock_delete(alarmClock_set *alarm_clock_set);
};

#endif // CLOCK_MAIN_ALARMCLOCK_FORM_H
