#include <QSyntaxHighlighter>

#include "clock_setting_dialog.h"
#include "ui_clock_setting_dialog.h"

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

bool Clock_Setting_Dialog::get_is_hide_when_mainWindow_clockButton_click()
{
    return m_is_hide_when_mainWindow_clockButton_click;
}
bool Clock_Setting_Dialog::get_is_sendTips_when_window_will_be_close()
{
    return m_is_sendTips_when_window_will_be_close;
}
bool Clock_Setting_Dialog::get_is_hide_when_app_start()
{
    return m_is_hide_when_app_start;
}

void Clock_Setting_Dialog::set_is_hide_when_mainWindow_clockButton_click(bool is_hide_when_mainWindow_clockButton_click)
{
    m_is_hide_when_mainWindow_clockButton_click = is_hide_when_mainWindow_clockButton_click;
    show_data();
}
void Clock_Setting_Dialog::set_is_sendTips_when_window_will_be_close(bool is_sendTips_when_window_will_be_close)
{
    m_is_sendTips_when_window_will_be_close = is_sendTips_when_window_will_be_close;
    show_data();
}
void Clock_Setting_Dialog::set_is_hide_when_app_start(bool is_hide_when_app_start)
{
    m_is_hide_when_app_start = is_hide_when_app_start;
    show_data();
}

void Clock_Setting_Dialog::save_data()
{
    save_timer_data();

    m_is_sendTips_when_window_will_be_close =
            ui->checkBox_is_sendTips_when_window_will_be_close->isChecked();
    m_is_hide_when_mainWindow_clockButton_click =
            ui->checkBox_is_hide_when_mainWindow_clockButton_click->isChecked();
    m_is_hide_when_app_start =
            ui->checkBox_is_hide_when_app_start->isChecked();
}
void Clock_Setting_Dialog::show_data()
{
    show_timer_data();

    show_alarm_clock_data();

    ui->checkBox_is_sendTips_when_window_will_be_close
            ->setChecked(m_is_sendTips_when_window_will_be_close);
    ui->checkBox_is_hide_when_mainWindow_clockButton_click
            ->setChecked(m_is_hide_when_mainWindow_clockButton_click);
    ui->checkBox_is_hide_when_app_start
            ->setChecked(m_is_hide_when_app_start);
}

void Clock_Setting_Dialog::show_timer_data()
{
    ui->lineEdit_command_path->setText(QString(m_timer_form_data.timer_command_path.c_str()));
    ui->lineEdit_music_path->setText(QString(m_timer_form_data.timer_music_path.c_str()));
    ui->textEdit_timer_message->setText(QString(m_timer_form_data.timer_message.c_str()));
}

void Clock_Setting_Dialog::save_timer_data()
{
    m_timer_form_data.timer_command_path = ui->lineEdit_command_path->text().toStdString();
    m_timer_form_data.timer_music_path = ui->lineEdit_music_path->text().toStdString();
    m_timer_form_data.timer_message = ui->textEdit_timer_message->toPlainText().toStdString();
}

void Clock_Setting_Dialog::show_alarm_clock_data()
{
    ui->lineEdit_alarm_clock_default_name->
            setText(QString(m_alarm_clock_default_setting.m_alarm_clock_data.str_alarm_clock_name.c_str()));
    ui->lineEdit_alarm_clock_default_time->
            setText(QString(m_alarm_clock_default_setting.get_str_time().c_str()));
    ui->lineEdit_alarm_clock_default_command_path->
            setText(QString(m_alarm_clock_default_setting.m_alarm_clock_data.str_command_path.c_str()));
    ui->lineEdit_alarm_clock_default_music_path->
            setText(QString(m_alarm_clock_default_setting.m_alarm_clock_data.str_music_path.c_str()));
    ui->textEdit_alarm_clock_default_message->
            setText(QString(m_alarm_clock_default_setting.m_alarm_clock_data.str_message.c_str()));
    ui->textEdit_alarm_clock_default_choose_range->
            setText(QString(m_alarm_clock_default_setting.get_str_timeType_choose_range().c_str()));
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
    new_alarm_clock_dialog->set_alarm_clock(m_alarm_clock_default_setting);
    if(new_alarm_clock_dialog->exec() == 1)
    {
        m_alarm_clock_default_setting = new_alarm_clock_dialog->get_alarm_clock_setting();
        show_alarm_clock_data();
    }
}

void Clock_Setting_Dialog::on_pushButton_choose_command_clicked()
{
    QString command_path = QFileDialog::getOpenFileName(this);
    ui->lineEdit_command_path->setText(command_path);
}

void Clock_Setting_Dialog::on_pushButton_choose_music_clicked()
{
    QString music_path = QFileDialog::getOpenFileName(this);
    ui->lineEdit_music_path->setText(music_path);
}
