#include <iostream>

#include "clock_choose_alarm_clock_time_form.h"
#include "ui_clock_choose_alarm_clock_time_form.h"

using namespace std;

const int m_arr_int_month_days[] = {30, 29, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};

Clock_Choose_Alarm_Clock_Time_Form::Clock_Choose_Alarm_Clock_Time_Form(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Clock_Choose_Alarm_Clock_Time_Form)
{
    ui->setupUi(this);
}

Clock_Choose_Alarm_Clock_Time_Form::~Clock_Choose_Alarm_Clock_Time_Form()
{
    delete ui;
}

int get_month_day(int month)
{
    return m_arr_int_month_days[month - 1];
}

void Clock_Choose_Alarm_Clock_Time_Form::set_time_choose_disabled_for_year()
{
    ui->frame_month->setDisabled(false);
}
void Clock_Choose_Alarm_Clock_Time_Form::set_time_choose_disabled_for_day()
{
    ui->frame_month->setDisabled(true);
}

void Clock_Choose_Alarm_Clock_Time_Form::set_time(int month, int day, int hour, int min)
{
    ui->spinBox_month->setValue(month);
    ui->spinBox_day->setValue(day);
    ui->spinBox_hour->setValue(hour);
    ui->spinBox_min->setValue(min);
}
void Clock_Choose_Alarm_Clock_Time_Form::get_time(int* p_month, int* p_day, int* p_hour, int* p_min)
{
    *p_month = ui->spinBox_month->value();
    *p_day = ui->spinBox_day->value();
    *p_hour = ui->spinBox_hour->value();
    *p_min = ui->spinBox_min->value();
}

void Clock_Choose_Alarm_Clock_Time_Form::on_spinBox_month_valueChanged(int arg1)
{
    if(arg1 < 1)
    {
        ui->spinBox_month->setValue(12);
        return;
    }
    if(arg1 > 12)
    {
        ui->spinBox_month->setValue(1);
        return;
    }
}

void Clock_Choose_Alarm_Clock_Time_Form::on_spinBox_day_valueChanged(int arg1)
{
    int month = ui->spinBox_month->value();
    if(arg1 < 1)
    {
        ui->spinBox_month->setValue(month - 1);
        ui->spinBox_day->setValue(get_month_day(ui->spinBox_month->value()));
        return;
    }
    else if(arg1 > get_month_day(month))
    {
        ui->spinBox_month->setValue(month + 1);
        ui->spinBox_day->setValue(1);
    }
}

void Clock_Choose_Alarm_Clock_Time_Form::on_spinBox_hour_valueChanged(int arg1)
{
    if(arg1 > 23)
    {
        ui->spinBox_hour->setValue(0);
        ui->spinBox_day->setValue(ui->spinBox_day->value() + 1);
    }
    else if(arg1 < 0)
    {
        ui->spinBox_hour->setValue(23);
        ui->spinBox_day->setValue(ui->spinBox_day->value() - 1);
    }
}

void Clock_Choose_Alarm_Clock_Time_Form::on_spinBox_min_valueChanged(int arg1)
{
    if(arg1 > 59)
    {
        ui->spinBox_min->setValue(0);
        ui->spinBox_hour->setValue(ui->spinBox_hour->value() + 1);
    }
    else if(arg1 < 0)
    {
        ui->spinBox_min->setValue(59);
        ui->spinBox_hour->setValue(ui->spinBox_hour->value() - 1);
    }
}
