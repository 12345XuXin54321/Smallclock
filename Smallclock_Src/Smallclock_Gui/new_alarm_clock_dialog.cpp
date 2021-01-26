#include "new_alarm_clock_dialog.h"
#include "ui_new_alarm_clock_dialog.h"

New_Alarm_Clock_Dialog::New_Alarm_Clock_Dialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::New_Alarm_Clock_Dialog)
{
    ui->setupUi(this);
}

New_Alarm_Clock_Dialog::~New_Alarm_Clock_Dialog()
{
    delete ui;
}

void New_Alarm_Clock_Dialog::set_week_days_choose(Alarm_Clock alarm_clock_setting)
{
    ui->checkBox_Mon->setChecked(false);
    ui->checkBox_Tues->setChecked(false);
    ui->checkBox_Wed->setChecked(false);
    ui->checkBox_Thur->setChecked(false);
    ui->checkBox_Fri->setChecked(false);
    ui->checkBox_Sat->setChecked(false);
    ui->checkBox_Sun->setChecked(false);

    for(auto choose_day : alarm_clock_setting.m_alarm_clock_data.vector_alarm_time_range_a_week)
    {
        switch (choose_day)
        {
            case Alarm_Clock::Mon:
            {
                ui->checkBox_Mon->setChecked(true);
                break;
            }
            case Alarm_Clock::Tues:
            {
                ui->checkBox_Tues->setChecked(true);
                break;
            }
            case Alarm_Clock::Wed:
            {
                ui->checkBox_Wed->setChecked(true);
                break;
            }
            case Alarm_Clock::Thur:
            {
                ui->checkBox_Thur->setChecked(true);
                break;
            }
            case Alarm_Clock::Fri:
            {
                ui->checkBox_Fri->setChecked(true);
                break;
            }
            case Alarm_Clock::Sat:
            {
                ui->checkBox_Sat->setChecked(true);
                break;
            }
            case Alarm_Clock::Sun:
            {
                ui->checkBox_Sun->setChecked(true);
                break;
            }
        }
    }
}
void New_Alarm_Clock_Dialog::get_week_days_choose(Alarm_Clock* alarm_clock_setting)
{
    alarm_clock_setting->m_alarm_clock_data.vector_alarm_time_range_a_week.clear();
    if(ui->checkBox_Mon->isChecked() == true)
    {
        alarm_clock_setting->m_alarm_clock_data.vector_alarm_time_range_a_week.push_back(Alarm_Clock::Mon);
    }
    if(ui->checkBox_Tues->isChecked() == true)
    {
        alarm_clock_setting->m_alarm_clock_data.vector_alarm_time_range_a_week.push_back(Alarm_Clock::Tues);
    }
    if(ui->checkBox_Wed->isChecked() == true)
    {
        alarm_clock_setting->m_alarm_clock_data.vector_alarm_time_range_a_week.push_back(Alarm_Clock::Wed);
    }
    if(ui->checkBox_Thur->isChecked() == true)
    {
        alarm_clock_setting->m_alarm_clock_data.vector_alarm_time_range_a_week.push_back(Alarm_Clock::Thur);
    }
    if(ui->checkBox_Fri->isChecked() == true)
    {
        alarm_clock_setting->m_alarm_clock_data.vector_alarm_time_range_a_week.push_back(Alarm_Clock::Fri);
    }
    if(ui->checkBox_Sat->isChecked() == true)
    {
        alarm_clock_setting->m_alarm_clock_data.vector_alarm_time_range_a_week.push_back(Alarm_Clock::Sat);
    }
    if(ui->checkBox_Sun->isChecked() == true)
    {
        alarm_clock_setting->m_alarm_clock_data.vector_alarm_time_range_a_week.push_back(Alarm_Clock::Sun);
    }
}

void New_Alarm_Clock_Dialog::set_range_choose(Alarm_Clock alarm_clock_setting)
{
    switch(alarm_clock_setting.m_alarm_clock_data.timeType_choose_range)
    {
        case Alarm_Clock::Time_Type::no_range:
        {
            ui->radioButton_no_range->click();
            break;
        }
        case Alarm_Clock::Time_Type::range_a_day:
        {
            ui->radioButton_range_a_day->click();
            break;
        }
        case Alarm_Clock::Time_Type::range_a_week:
        {
            ui->radioButton_range_a_week->click();
            break;
        }
        case Alarm_Clock::Time_Type::range_a_year:
        {
            ui->radioButton_range_a_year->click();
            break;
        }
    }
}
void New_Alarm_Clock_Dialog::get_range_choose(Alarm_Clock* alarm_clock_setting)
{
    if(ui->radioButton_no_range->isChecked() == true)
    {
        alarm_clock_setting->m_alarm_clock_data.timeType_choose_range = Alarm_Clock::Time_Type::no_range;
    }
    else if(ui->radioButton_range_a_day->isChecked() == true)
    {
        alarm_clock_setting->m_alarm_clock_data.timeType_choose_range = Alarm_Clock::Time_Type::range_a_day;
    }
    else if(ui->radioButton_range_a_week->isChecked() == true)
    {
        alarm_clock_setting->m_alarm_clock_data.timeType_choose_range = Alarm_Clock::Time_Type::range_a_week;
    }
    else if(ui->radioButton_range_a_year->isChecked() == true)
    {
        alarm_clock_setting->m_alarm_clock_data.timeType_choose_range = Alarm_Clock::Time_Type::range_a_year;
    }
}

void New_Alarm_Clock_Dialog::set_alarm_clock(Alarm_Clock setting_data)
{
    m_alarm_clock_setting = setting_data;

    ui->alarm_clock_choose_time_form->set_time(setting_data.m_alarm_clock_data.int_month,
                                               setting_data.m_alarm_clock_data.int_day,
                                               setting_data.m_alarm_clock_data.int_hour,
                                               setting_data.m_alarm_clock_data.int_min);

    set_week_days_choose(setting_data);

    ui->dateTimeEdit_no_range->
            setDateTime(QDateTime::fromSecsSinceEpoch(setting_data.m_alarm_clock_data.longlongint_accurate_time));

    set_range_choose(setting_data);

    ui->lineEdit_alarm_clock_name->setText(QString(setting_data.m_alarm_clock_data.str_alarm_clock_name.c_str()));
    ui->lineEdit_alarm_clock_command_path->setText(QString(setting_data.m_alarm_clock_data.str_command_path.c_str()));
    ui->lineEdit_alarm_clock_music_path->setText(QString(setting_data.m_alarm_clock_data.str_music_path.c_str()));
    ui->textEdit_alarm_clock_message->setText(QString(setting_data.m_alarm_clock_data.str_message.c_str()));
}
Alarm_Clock New_Alarm_Clock_Dialog::get_alarm_clock_setting()
{
    get_week_days_choose(&m_alarm_clock_setting);
    ui->alarm_clock_choose_time_form->get_time(&(m_alarm_clock_setting.m_alarm_clock_data.int_month),
                                               &(m_alarm_clock_setting.m_alarm_clock_data.int_day),
                                               &(m_alarm_clock_setting.m_alarm_clock_data.int_hour),
                                               &(m_alarm_clock_setting.m_alarm_clock_data.int_min));

    m_alarm_clock_setting.m_alarm_clock_data.longlongint_accurate_time =
            ui->dateTimeEdit_no_range->dateTime().toSecsSinceEpoch();

    get_range_choose(&m_alarm_clock_setting);

    m_alarm_clock_setting.m_alarm_clock_data.str_alarm_clock_name = ui->lineEdit_alarm_clock_name->text().toStdString();
    m_alarm_clock_setting.m_alarm_clock_data.str_command_path = ui->lineEdit_alarm_clock_command_path->text().toStdString();
    m_alarm_clock_setting.m_alarm_clock_data.str_music_path = ui->lineEdit_alarm_clock_music_path->text().toStdString();
    m_alarm_clock_setting.m_alarm_clock_data.str_message = ui->textEdit_alarm_clock_message->toPlainText().toStdString();

    return m_alarm_clock_setting;
}

void New_Alarm_Clock_Dialog::on_radioButton_range_a_year_clicked()
{
    ui->alarm_clock_choose_time_form->setEnabled(true);
    ui->alarm_clock_choose_time_form->set_time_choose_disabled_for_year();
    ui->frame_choose_week_days->setEnabled(false);
    ui->dateTimeEdit_no_range->setEnabled(false);
}

void New_Alarm_Clock_Dialog::on_radioButton_range_a_week_clicked()
{
    ui->alarm_clock_choose_time_form->setEnabled(true);
    ui->alarm_clock_choose_time_form->set_time_choose_disabled_for_day();
    ui->frame_choose_week_days->setEnabled(true);
    ui->dateTimeEdit_no_range->setEnabled(false);
}

void New_Alarm_Clock_Dialog::on_radioButton_range_a_day_clicked()
{
    ui->alarm_clock_choose_time_form->setEnabled(true);
    ui->alarm_clock_choose_time_form->set_time_choose_disabled_for_day();
    ui->frame_choose_week_days->setEnabled(false);
    ui->dateTimeEdit_no_range->setEnabled(false);
}

void New_Alarm_Clock_Dialog::on_radioButton_no_range_clicked()
{
    ui->alarm_clock_choose_time_form->setEnabled(false);
    ui->frame_choose_week_days->setEnabled(false);
    ui->dateTimeEdit_no_range->setEnabled(true);
}

void New_Alarm_Clock_Dialog::on_pushButton_alarm_clock_choose_command_clicked()
{
    QString command_path = QFileDialog::getOpenFileName(this);
    ui->lineEdit_alarm_clock_command_path->setText(command_path);
}

void New_Alarm_Clock_Dialog::on_pushButton_alarm_clock_choose_music_clicked()
{
    QString music_path = QFileDialog::getOpenFileName(this);
    ui->lineEdit_alarm_clock_music_path->setText(music_path);
}
