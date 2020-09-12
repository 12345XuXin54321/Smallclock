#include "clocktimer_choose_time_dialog.h"
#include "ui_clocktimer_choose_time_dialog.h"

ClockTimer_Choose_Time_Dialog::ClockTimer_Choose_Time_Dialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ClockTimer_Choose_Time_Dialog)
{
    ui->setupUi(this);
}

ClockTimer_Choose_Time_Dialog::~ClockTimer_Choose_Time_Dialog()
{
    delete ui;
}

__int128 ClockTimer_Choose_Time_Dialog::get_hour()
{
    return ui->spinBox_hour->value();
}
__int128 ClockTimer_Choose_Time_Dialog::get_min()
{
    return ui->spinBox_min->value();
}
__int128 ClockTimer_Choose_Time_Dialog::get_sec()
{
    return ui->spinBox_sec->value();
}
