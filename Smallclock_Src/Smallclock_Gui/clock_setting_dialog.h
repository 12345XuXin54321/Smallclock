#ifndef CLOCK_SETTING_DIALOG_H
#define CLOCK_SETTING_DIALOG_H

#include <iostream>

#include <QDialog>
#include <QAbstractButton>

#include "new_alarm_clock_dialog.h"

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
    string get_timer_command();
    string get_timer_message();

    void set_timer_command(string str_command);
    void set_timer_message(string str_message);

    Alarm_Clock get_alarm_default_setting();
    void set_alarm_default_setting(Alarm_Clock alarm_clock_default);

private slots:

    void on_pushButton_set_default_alarm_clock_clicked();

private:
    Ui::Clock_Setting_Dialog *ui;

    string m_string_timer_command;
    string m_string_timer_message;

    Alarm_Clock m_alarm_clock_setting;

    void save_data();
    void show_data();

    void show_alarm_setting(Alarm_Clock setting);
};

#endif // CLOCK_SETTING_DIALOG_H
