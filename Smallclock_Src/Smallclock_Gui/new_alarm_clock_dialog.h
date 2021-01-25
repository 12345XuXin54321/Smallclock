#ifndef NEW_ALARM_CLOCK_DIALOG_H
#define NEW_ALARM_CLOCK_DIALOG_H

#include <iostream>
#include <vector>

#include <QDialog>
#include <QDateTime>
#include <QFileDialog>

#include "../Smallclock_Base/App_Data_Save.h"

#include "clock_choose_alarm_clock_time_form.h"

using namespace std;

struct Alarm_Clock
{
    enum Time_Type
    {
        range_a_day,
        range_a_week,
        range_a_year,
        no_range
    };

    enum Day_In_Week
    {
        Mon = 1,
        Tues = 2,
        Wed = 3,
        Thur = 4,
        Fri = 5,
        Sat = 6,
        Sun = 7
    };

    string m_str_alarm_clock_name = "新闹钟";
    string m_str_message;
    string m_str_music;
    string m_str_command;

    Time_Type m_timeType_choose_range = Time_Type::range_a_day;

    /// the number of seconds since
    /// 1970-01-01T00:00:00 Universal Coordinated Time.
    long long int m_longlongint_accurate_time = QDateTime::currentDateTime().toSecsSinceEpoch();
    int m_month = QDate::currentDate().month();
    int m_day = QDate::currentDate().day();
    int m_hour = QTime::currentTime().hour();
    int m_min = QTime::currentTime().minute();

    bool m_is_turn_on = true;
    bool m_is_remind = false;

    vector<Day_In_Week> m_vector_alarm_time_range_a_week;

    string get_str_timeType_choose_range()
    {
        switch(m_timeType_choose_range)
        {
            case Time_Type::no_range:
            {
                return "仅一次";
            }
            case Time_Type::range_a_day:
            {
                return "每天一次";
            }
            case Time_Type::range_a_week:
            {
                string return_data;
                return_data.reserve(128);
                size_t p_vector = 0;
                for(auto choose_days : m_vector_alarm_time_range_a_week)
                {
                    switch(choose_days)
                    {
                        case Day_In_Week::Mon:
                        {
                            return_data += "周一";
                            break;
                        }
                        case Day_In_Week::Tues:
                        {
                            return_data += "周二";
                            break;
                        }
                        case Day_In_Week::Wed:
                        {
                            return_data += "周三";
                            break;
                        }
                        case Day_In_Week::Thur:
                        {
                            return_data += "周四";
                            break;
                        }
                        case Day_In_Week::Fri:
                        {
                            return_data += "周五";
                            break;
                        }
                        case Day_In_Week::Sat:
                        {
                            return_data += "周六";
                            break;
                        }
                        case Day_In_Week::Sun:
                        {
                            return_data += "周日";
                            break;
                        }
                    }
                    if(p_vector < m_vector_alarm_time_range_a_week.size() - 1)
                    {
                        if(p_vector % 2 == 0)
                        {
                            return_data += "　";
                        }
                        else
                        {
                            return_data += "\n";
                        }
                        p_vector++;
                    }
                }
                return return_data;
            }
            case Time_Type::range_a_year:
            {
                return "每年一次";
            }
        }
        return string();
    }

    string get_str_time()
    {
        switch(m_timeType_choose_range)
        {
            case Time_Type::no_range:
            {
                auto dateTime = QDateTime::fromSecsSinceEpoch(m_longlongint_accurate_time);
                return to_string(dateTime.date().year()) + "年"
                        + to_string(dateTime.date().month()) + "月"
                        + to_string(dateTime.date().day()) + "日"
                        + to_string(dateTime.time().hour()) + "时"
                        + to_string(dateTime.time().minute()) + "分";
            }
            case Time_Type::range_a_day:
            case Time_Type::range_a_week:
            {
                return to_string(m_hour) + "时"
                        + to_string(m_min) + "分";
            }
            case Time_Type::range_a_year:
            {
                return to_string(m_month) + "月"
                        + to_string(m_day) + "日"
                        + to_string(m_hour) + "时"
                        + to_string(m_min) + "分";
            }
        }
        return string();
    }

    string get_str_alarm_clock_state()
    {
        if(m_is_turn_on == false)
        {
            return "未开启";
        }
        else
        {
            if(m_is_remind == true)
            {
                return "提醒中";
            }
            else
            {
                return "已开启";
            }
        }
    }

    void write_save_data(App_Data_Save* data_save)
    {
        data_save->write_next_data(m_str_alarm_clock_name);
        data_save->write_next_data(m_str_message);
        data_save->write_next_data(m_str_music);
        data_save->write_next_data(m_str_command);

        data_save->write_next_data<Alarm_Clock::Time_Type>(m_timeType_choose_range);
        data_save->write_next_data<long long int>(m_longlongint_accurate_time);
        data_save->write_next_data<int>(m_month);
        data_save->write_next_data<int>(m_day);
        data_save->write_next_data<int>(m_hour);
        data_save->write_next_data<int>(m_min);

        data_save->write_next_data<bool>(m_is_turn_on);

        data_save->write_next_data<int>(m_vector_alarm_time_range_a_week.size());
        for(size_t i = 0; i < m_vector_alarm_time_range_a_week.size(); i++)
        {
            data_save->write_next_data<Alarm_Clock::Day_In_Week>(
                        m_vector_alarm_time_range_a_week[i]
                        );
        }
    }
    void load_save_data(App_Data_Save* data_save)
    {
        m_str_alarm_clock_name = data_save->read_next_data();
        m_str_message = data_save->read_next_data();
        m_str_music = data_save->read_next_data();
        m_str_command = data_save->read_next_data();

        m_timeType_choose_range = data_save->read_next_data<Alarm_Clock::Time_Type>();

        m_longlongint_accurate_time = data_save->read_next_data<long long int>();
        m_month = data_save->read_next_data<int>();
        m_day = data_save->read_next_data<int>();
        m_hour = data_save->read_next_data<int>();
        m_min = data_save->read_next_data<int>();

        m_is_turn_on = data_save->read_next_data<bool>();

        int day_in_week_length = data_save->read_next_data<int>();
        for(int i = 0; i < day_in_week_length; i++)
        {
            m_vector_alarm_time_range_a_week.push_back(
                        data_save->read_next_data<Alarm_Clock::Day_In_Week>()
                        );
        }
    }
};

namespace Ui {
    class New_Alarm_Clock_Dialog;
}

class New_Alarm_Clock_Dialog : public QDialog
{
    Q_OBJECT

public:
    explicit New_Alarm_Clock_Dialog(QWidget *parent = nullptr);
    ~New_Alarm_Clock_Dialog();

    void set_alarm_clock(Alarm_Clock setting_data);
    Alarm_Clock get_alarm_clock_setting();

private slots:
    void on_radioButton_range_a_year_clicked();

    void on_radioButton_range_a_week_clicked();

    void on_radioButton_range_a_day_clicked();

    void on_radioButton_no_range_clicked();

    void on_pushButton_alarm_clock_choose_command_clicked();

    void on_pushButton_alarm_clock_choose_music_clicked();

private:
    Ui::New_Alarm_Clock_Dialog *ui;

    Alarm_Clock m_alarm_clock_setting;

    void set_week_days_choose(Alarm_Clock alarm_clock_setting);
    void get_week_days_choose(Alarm_Clock* alarm_clock_setting);

    void set_range_choose(Alarm_Clock alarm_clock_setting);
    void get_range_choose(Alarm_Clock* alarm_clock_setting);
};

#endif // NEW_ALARM_CLOCK_DIALOG_H
