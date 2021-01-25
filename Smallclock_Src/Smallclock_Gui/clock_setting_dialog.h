#ifndef CLOCK_SETTING_DIALOG_H
#define CLOCK_SETTING_DIALOG_H

#include <iostream>

#include <QDialog>
#include <QAbstractButton>

#include "new_alarm_clock_dialog.h"
#include "clock_main_timer_form.h"

using namespace std;

namespace Ui {
    class Clock_Setting_Dialog;
}

class Clock_Setting_Dialog : public QDialog
{
    Q_OBJECT

public:
    explicit Clock_Setting_Dialog(QWidget *parent = nullptr);
    ~Clock_Setting_Dialog();

    int exec();

    Clock_Main_Timer_Form::Timer_Form_Data get_main_timer_form_setting()
    {
        return m_timer_form_data;
    }
    void show_main_timer_form_setting(Clock_Main_Timer_Form::Timer_Form_Data timer_data)
    {
        m_timer_form_data = timer_data;
        show_timer_data();
    }
//    string get_timer_command()
//    {
//        return m_string_timer_command;
//    }
//    string get_timer_music()
//    {
//        return m_string_timer_music;
//    }
//    string get_timer_message()
//    {
//        return m_string_timer_message;
//    }

//    void set_timer_command(string str_command)
//    {
//        m_string_timer_command = str_command;
//        show_data();
//    }
//    void set_timer_music(string str_music)
//    {
//        m_string_timer_music = str_music;
//        show_data();
//    }
//    void set_timer_message(string str_message)
//    {
//        m_string_timer_message = str_message;
//        show_data();
//    }

    Alarm_Clock get_alarm_default_setting();
    void set_alarm_default_setting(Alarm_Clock alarm_clock_default);

    bool get_is_hide_when_mainWindow_clockButton_click();
    bool get_is_sendTips_when_window_will_be_close();
    bool get_is_hide_when_app_start();

    void set_is_hide_when_mainWindow_clockButton_click(bool is_hide_when_mainWindow_clockButton_click);
    void set_is_sendTips_when_window_will_be_close(bool is_sendTips_when_window_will_be_close);
    void set_is_hide_when_app_start(bool is_hide_when_app_start);

private slots:

    void on_pushButton_set_default_alarm_clock_clicked();

    void on_pushButton_choose_command_clicked();

    void on_pushButton_choose_music_clicked();

private:
    Ui::Clock_Setting_Dialog *ui;

//    string m_string_timer_command;
//    string m_string_timer_music;
//    string m_string_timer_message;

    Clock_Main_Timer_Form::Timer_Form_Data m_timer_form_data;

    Alarm_Clock m_alarm_clock_setting;

    bool m_is_hide_when_mainWindow_clockButton_click = false;
    bool m_is_sendTips_when_window_will_be_close = false;
    bool m_is_hide_when_app_start = false;

    void save_data();
    void show_data();

    void show_timer_data();

    void save_timer_data();

    void show_alarm_setting(Alarm_Clock setting);
};

#endif // CLOCK_SETTING_DIALOG_H
