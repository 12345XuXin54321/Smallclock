#define Flickered_Remind_Time 400

#include "clock_number_display_form.h"
#include "ui_clock_number_display_form.h"

Clock_Number_Display_Form::Clock_Number_Display_Form(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Clock_Number_Display_Form)
{
    ui->setupUi(this);
    m_flickered_remind_timer = new QTimer();
    connect(m_flickered_remind_timer, SIGNAL(timeout()), this, SLOT(do_to_flickered_remind()));
}

Clock_Number_Display_Form::~Clock_Number_Display_Form()
{
    delete ui;

    delete m_flickered_remind_timer;
}

void Clock_Number_Display_Form::set_hour(int hour)
{
    int hour_all = hour % 100;
    ui->lcdNumber_hour_hundred->display(hour_all / 10);
    ui->lcdNumber_hour_ten->display(hour_all % 10);
}
void Clock_Number_Display_Form::set_min(int min)
{
    int min_all = min % 60;
    ui->lcdNumber_min_hundred->display(min_all / 10);
    ui->lcdNumber_min_ten->display(min_all % 10);
}
void Clock_Number_Display_Form::set_sec(int sec)
{
    int sec_all = sec % 60;
    ui->lcdNumber_sec_hundred->display(sec_all / 10);
    ui->lcdNumber_sec_ten->display(sec_all % 10);
}
void Clock_Number_Display_Form::set_msec(int msec)
{
    int msec_all = msec % 1000;
    int t = msec_all / 100;
    msec_all -= (t * 100);
    ui->lcdNumber_msec_thousand->display(t);

    int h = msec_all / 10;
    msec_all -= (h * 10);
    ui->lcdNumber_msec_hundred->display(h);

    ui->lcdNumber_msec_ten->display(msec_all);
}
void Clock_Number_Display_Form::set_time(__int128 usec)
{
    __int128 all_time_msec = usec / 1000;
    int hour = all_time_msec / (1000 * 60 * 60);
    set_hour(hour);
    all_time_msec -= (hour * (1000 * 60 * 60));

    int min = all_time_msec / (1000 * 60);
    set_min(min);
    all_time_msec -= (min * (1000 * 60));

    int sec = all_time_msec / (1000);
    set_sec(sec);
    all_time_msec -= (sec * 1000);

    set_msec(all_time_msec);
}

void Clock_Number_Display_Form::start_flickered_remind()
{
    do_to_flickered_remind();
    m_flickered_remind_timer->start(Flickered_Remind_Time);
}
void Clock_Number_Display_Form::stop_flickered_remind()
{
    m_flickered_remind_timer->stop();
    is_light = true;
    this->set_style(is_light);
}

void Clock_Number_Display_Form::do_to_flickered_remind()
{
    this->set_style_diaphaneity();
}

void Clock_Number_Display_Form::set_style_diaphaneity()
{
    if(is_light == true)
    {
        is_light = false;
    }
    else
    {
        is_light = true;
    }
    set_style(is_light);
}

void Clock_Number_Display_Form::set_style(bool value)
{
    if(value == false)
    {
        this->setStyleSheet("color: rgb(255, 93, 93);");
    }
    else
    {
        this->setStyleSheet("color: rgb(20, 20, 20);");
    }
}
