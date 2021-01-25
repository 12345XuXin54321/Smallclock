#include "clock_main_stopwatch_form.h"
#include "ui_clock_main_stopwatch_form.h"

const int m_const_int_flush_time = 53;

Clock_Main_Stopwatch_Form::Clock_Main_Stopwatch_Form(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Clock_Main_Stopwatch_Form)
{
    ui->setupUi(this);

    m_timer_flush = new QTimer(this);
    connect(m_timer_flush, SIGNAL(timeout()), this, SLOT(do_when_flush_timer_timeout()));
}

Clock_Main_Stopwatch_Form::~Clock_Main_Stopwatch_Form()
{
    delete ui;
}

void Clock_Main_Stopwatch_Form::stopwatch_start()
{
    m_is_stopwatch_start = true;
    ui->pushButton_stopwatch_start_stop->setText("暂停");
    qtimer_start();
}
void Clock_Main_Stopwatch_Form::stopwatch_stop()
{
    m_is_stopwatch_start = false;
    qtimer_stop();
    ui->pushButton_stopwatch_start_stop->setText("开始");
}
void Clock_Main_Stopwatch_Form::stopwatch_flush()
{
    ui->clock_number_display_form_stopwatch->
            set_time(m_int128_stopwatch_time_use +
                     (m_get_time.get_tv_time_now() - m_int128_stopwatch_begin_time));
}

void Clock_Main_Stopwatch_Form::qtimer_start()
{

    m_timer_flush->start(m_const_int_flush_time);

}
void Clock_Main_Stopwatch_Form::qtimer_stop()
{

    m_timer_flush->stop();

}

void Clock_Main_Stopwatch_Form::do_when_flush_timer_timeout()
{
    if(m_is_stopwatch_start == true)
    {
        stopwatch_flush();
    }
}

void Clock_Main_Stopwatch_Form::on_pushButton_stopwatch_start_stop_clicked()
{
    if(m_is_stopwatch_start == false)
    {
        m_int128_stopwatch_begin_time = m_get_time.get_tv_time_now();

        ui->clock_number_display_form_stopwatch
                ->set_time(m_int128_stopwatch_time_use + (m_get_time.get_tv_time_now() - m_int128_stopwatch_begin_time));
        stopwatch_start();
    }
    else
    {
        stopwatch_stop();

        long end_time = m_get_time.get_tv_time_now();

        ui->clock_number_display_form_stopwatch
                ->set_time(m_int128_stopwatch_time_use + (end_time - m_int128_stopwatch_begin_time));

        m_int128_stopwatch_time_use += (end_time - m_int128_stopwatch_begin_time);
        m_int128_stopwatch_begin_time = 0;
    }
}

void Clock_Main_Stopwatch_Form::on_pushButton_stopwatch_reset_clicked()
{
    stopwatch_stop();
    m_int128_stopwatch_time_use = 0;
    ui->clock_number_display_form_stopwatch->set_time(0);
}
