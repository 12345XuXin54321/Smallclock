#ifndef CLOCK_CHOOSE_ALARM_CLOCK_TIME_FORM_H
#define CLOCK_CHOOSE_ALARM_CLOCK_TIME_FORM_H

#include <QWidget>

namespace Ui {
    class Clock_Choose_Alarm_Clock_Time_Form;
}

class Clock_Choose_Alarm_Clock_Time_Form : public QWidget
{
    Q_OBJECT

public:
    explicit Clock_Choose_Alarm_Clock_Time_Form(QWidget *parent = nullptr);
    ~Clock_Choose_Alarm_Clock_Time_Form();

    void set_time_choose_disabled_for_year();
    void set_time_choose_disabled_for_day();

    void set_time(int month, int day, int hour, int min);
    void get_time(int* p_month, int* p_day, int* p_hour, int* p_min);

private slots:
    void on_spinBox_month_valueChanged(int arg1);

    void on_spinBox_day_valueChanged(int arg1);

    void on_spinBox_hour_valueChanged(int arg1);

    void on_spinBox_min_valueChanged(int arg1);

private:
    Ui::Clock_Choose_Alarm_Clock_Time_Form *ui;
};

#endif // CLOCK_CHOOSE_ALARM_CLOCK_TIME_FORM_H
