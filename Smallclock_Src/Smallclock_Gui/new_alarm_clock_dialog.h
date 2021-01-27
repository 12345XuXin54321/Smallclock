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
public:
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

    struct Alarm_Clock_Data
    {
        string str_alarm_clock_name = "新闹钟";
        string str_message;
        string str_music_path;
        string str_command_path;

        Time_Type timeType_choose_range = Time_Type::range_a_day;

        /// the number of seconds since
        /// 1970-01-01T00:00:00 Universal Coordinated Time.
        long long int longlongint_accurate_time = QDateTime::currentDateTime().toSecsSinceEpoch();
        int int_month = QDate::currentDate().month();
        int int_day = QDate::currentDate().day();
        int int_hour = QTime::currentTime().hour();
        int int_min = QTime::currentTime().minute();

        bool is_turn_on = true;

        vector<Day_In_Week> vector_alarm_time_range_a_week;

        void write_save_data(App_Data_Save* data_save)
        {
            data_save->write_next_data(str_alarm_clock_name);
            data_save->write_next_data(str_message);
            data_save->write_next_data(str_music_path);
            data_save->write_next_data(str_command_path);

            data_save->write_next_data<Alarm_Clock::Time_Type>(timeType_choose_range);
            data_save->write_next_data<long long int>(longlongint_accurate_time);
            data_save->write_next_data<int>(int_month);
            data_save->write_next_data<int>(int_day);
            data_save->write_next_data<int>(int_hour);
            data_save->write_next_data<int>(int_min);

            data_save->write_next_data<bool>(is_turn_on);

            data_save->write_next_data<int>(vector_alarm_time_range_a_week.size());
            for(size_t i = 0; i < vector_alarm_time_range_a_week.size(); i++)
            {
                data_save->write_next_data<Alarm_Clock::Day_In_Week>(
                            vector_alarm_time_range_a_week[i]
                            );
            }
        }
        void load_save_data(App_Data_Save* data_save)
        {
            str_alarm_clock_name = data_save->read_next_data();
            str_message = data_save->read_next_data();
            str_music_path = data_save->read_next_data();
            str_command_path = data_save->read_next_data();

            timeType_choose_range = data_save->read_next_data<Alarm_Clock::Time_Type>();

            longlongint_accurate_time = data_save->read_next_data<long long int>();
            int_month = data_save->read_next_data<int>();
            int_day = data_save->read_next_data<int>();
            int_hour = data_save->read_next_data<int>();
            int_min = data_save->read_next_data<int>();

            is_turn_on = data_save->read_next_data<bool>();

            int day_in_week_length = data_save->read_next_data<int>();
            for(int i = 0; i < day_in_week_length; i++)
            {
                vector_alarm_time_range_a_week.push_back(
                            data_save->read_next_data<Alarm_Clock::Day_In_Week>()
                            );
            }
        }

    } m_alarm_clock_data;

    string get_str_timeType_choose_range()
    {
        switch(m_alarm_clock_data.timeType_choose_range)
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
                    for(auto choose_days : m_alarm_clock_data.vector_alarm_time_range_a_week)
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
                        if(p_vector < m_alarm_clock_data.vector_alarm_time_range_a_week.size() - 1)
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
        switch(m_alarm_clock_data.timeType_choose_range)
        {
            case Time_Type::no_range:
                {
                    auto dateTime = QDateTime::fromSecsSinceEpoch(m_alarm_clock_data.longlongint_accurate_time);
                    return to_string(dateTime.date().year()) + "年"
                        + to_string(dateTime.date().month()) + "月"
                        + to_string(dateTime.date().day()) + "日"
                        + to_string(dateTime.time().hour()) + "时"
                        + to_string(dateTime.time().minute()) + "分";
                }
            case Time_Type::range_a_day:
            case Time_Type::range_a_week:
                {
                    return to_string(m_alarm_clock_data.int_hour) + "时"
                        + to_string(m_alarm_clock_data.int_min) + "分";
                }
            case Time_Type::range_a_year:
                {
                    return to_string(m_alarm_clock_data.int_month) + "月"
                        + to_string(m_alarm_clock_data.int_day) + "日"
                        + to_string(m_alarm_clock_data.int_hour) + "时"
                        + to_string(m_alarm_clock_data.int_min) + "分";
                }
        }
        return string();
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
