#include <QSyntaxHighlighter>

#include "clock_setting_dialog.h"
#include "ui_clock_setting_dialog.h"

Alarm_Clock temp_alarm_clock_seting;

Clock_Setting_Dialog::Clock_Setting_Dialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Clock_Setting_Dialog)
{
    ui->setupUi(this);
}

Clock_Setting_Dialog::~Clock_Setting_Dialog()
{
    delete ui;
}

string Clock_Setting_Dialog::get_timer_command()
{
    return m_string_timer_command;
}
string Clock_Setting_Dialog::get_timer_message()
{
    return m_string_timer_message;
}

void Clock_Setting_Dialog::set_timer_command(string str_command)
{
    m_string_timer_command = str_command;
    show_data();
}
void Clock_Setting_Dialog::set_timer_message(string str_message)
{
    m_string_timer_message = str_message;
    show_data();
}

Alarm_Clock Clock_Setting_Dialog::get_alarm_default_setting()
{
    return m_alarm_clock_setting;
}
void Clock_Setting_Dialog::set_alarm_default_setting(Alarm_Clock alarm_clock_default)
{
    m_alarm_clock_setting = alarm_clock_default;
    show_data();
}

void Clock_Setting_Dialog::save_data()
{
    m_string_timer_command = ui->textEdit_timer_command->toPlainText().toStdString();
    m_string_timer_message = ui->textEdit_timer_message->toPlainText().toStdString();
    m_alarm_clock_setting = temp_alarm_clock_seting;
}
void Clock_Setting_Dialog::show_data()
{
    ui->textEdit_timer_command->setText(QString(m_string_timer_command.c_str()));
    ui->textEdit_timer_message->setText(QString(m_string_timer_message.c_str()));
    show_alarm_setting(m_alarm_clock_setting);
    temp_alarm_clock_seting = m_alarm_clock_setting;
}

void Clock_Setting_Dialog::show_alarm_setting(Alarm_Clock setting)
{
    ui->lineEdit_alarm_clock_default_name->
            setText(QString(setting.m_str_alarm_clock_name.c_str()));
    ui->lineEdit_alarm_clock_default_time->
            setText(QString(setting.get_str_time().c_str()));
    ui->textEdit_alarm_clock_default_command->
            setText(QString(setting.m_str_command.c_str()));
    ui->textEdit_alarm_clock_default_message->
            setText(QString(setting.m_str_message.c_str()));
    ui->textEdit_alarm_clock_default_choose_range->
            setText(QString(setting.get_str_timeType_choose_range().c_str()));
}

int Clock_Setting_Dialog::exec()
{
    show_data();
    int click_button = QDialog::exec();
    if(click_button == 1)
    {
        save_data();
    }
    return click_button;
}

void Clock_Setting_Dialog::on_pushButton_set_default_alarm_clock_clicked()
{
    New_Alarm_Clock_Dialog *new_alarm_clock_dialog = new New_Alarm_Clock_Dialog();
    new_alarm_clock_dialog->set_alarm_clock(temp_alarm_clock_seting);
    if(new_alarm_clock_dialog->exec() == 1)
    {
        temp_alarm_clock_seting = new_alarm_clock_dialog->get_alarm_clock_setting();
        show_alarm_setting(temp_alarm_clock_seting);
    }
}
