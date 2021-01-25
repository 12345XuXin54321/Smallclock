#include "../Smallclock_Base/Music_Player.h"

#include "clocktimer_choose_time_dialog.h"
#include "Message_Window_Show.h"

#include "clock_main_timer_form.h"
#include "ui_clock_main_timer_form.h"

const int m_const_int_flush_time = 53;

Clock_Main_Timer_Form::Clock_Main_Timer_Form(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Clock_Main_Timer_Form)
{
    ui->setupUi(this);

    m_timer_flush = new QTimer(this);
    connect(m_timer_flush, SIGNAL(timeout()), this, SLOT(do_when_flush_timer_timeout()));
}

Clock_Main_Timer_Form::~Clock_Main_Timer_Form()
{
    delete ui;
}

void Clock_Main_Timer_Form::clocktimer_initialization()
{
    m_int128_timer_duration = 0;
    m_int128_timer_begin_time = 0;
    m_int128_timer_time_use = 0;
    m_is_clocktimer_start = false;
    m_is_clocktimer_end = false;
    m_is_clocktimer_reminding = true;
    ui->clock_number_display_form_timer->set_time(0);
    ui->pushButton_timer_start_stop->setText("开始");
    ui->pushButton_timer_start_stop->setDisabled(true);
}
void Clock_Main_Timer_Form::clocktimer_start()
{
    m_is_clocktimer_start = true;
    ui->pushButton_timer_start_stop->setText("暂停");
    qtimer_start();
}
void Clock_Main_Timer_Form::clocktimer_stop()
{
    m_is_clocktimer_start = false;
    qtimer_stop();
    ui->pushButton_timer_start_stop->setText("开始");
}
void Clock_Main_Timer_Form::clocktimer_end()
{
    m_is_clocktimer_end = true;
    ui->pushButton_timer_start_stop->setText("停止");
    ui->clock_number_display_form_timer->start_flickered_remind();
}

void Clock_Main_Timer_Form::clocktimer_flush()
{
    __int128 time_use = m_int128_timer_time_use + (m_get_time.get_tv_time_now() - m_int128_timer_begin_time);
    if(m_int128_timer_duration - time_use > 0)
    {
        ui->clock_number_display_form_timer->set_time(m_int128_timer_duration - time_use);
    }
    else
    {
        ui->clock_number_display_form_timer->set_time(time_use - m_int128_timer_duration);
        if(m_is_clocktimer_end == false)
        {
            clocktimer_end();
            do_when_clock_timer_end();
        }
    }
}

void Clock_Main_Timer_Form::clocktimer_start_remind()
{
    Message_Window_Show::showMessageWindow("计时器已到时",
                                           m_timer_form_data.timer_message.c_str());
    if(m_timer_form_data.timer_command_path.size() > 0)
    {
        system((m_timer_form_data.timer_command_path + " &").c_str());
    }
    if(m_timer_form_data.timer_music_path.size() > 0)
    {
        m_music_player.start_play(m_timer_form_data.timer_music_path);
    }
}
void Clock_Main_Timer_Form::clocktimer_stop_remind()
{
    ui->clock_number_display_form_timer->stop_flickered_remind();
    m_music_player.stop_play();
}

void Clock_Main_Timer_Form::do_when_clock_timer_end()
{
    m_is_clocktimer_reminding = true;

    clocktimer_start_remind();

    cout << "ding----" << endl;
}

void Clock_Main_Timer_Form::qtimer_start()
{
    if(m_is_clocktimer_start == true)
    {
        m_timer_flush->start(m_const_int_flush_time);
    }
}
void Clock_Main_Timer_Form::qtimer_stop()
{
    if(m_is_clocktimer_start == false)
    {
        m_timer_flush->stop();
    }
}


void Clock_Main_Timer_Form::do_when_flush_timer_timeout()
{
    if(m_is_clocktimer_start == true)
    {
        clocktimer_flush();
    }
}

void Clock_Main_Timer_Form::on_pushButton_timer_set_clicked()
{
    ClockTimer_Choose_Time_Dialog *choose_time_dialog = new ClockTimer_Choose_Time_Dialog(this);
    //0 -> 取消 1 -> 确认
    int click_button = choose_time_dialog->exec();
    if(click_button == 1)
    {
        m_int128_timer_duration = choose_time_dialog->get_hour() * (60 * 60) * (1000 * 1000) +
                choose_time_dialog->get_min() * 60 * (1000 * 1000) +
                choose_time_dialog->get_sec() * (1000 * 1000);
        ui->clock_number_display_form_timer->set_time(m_int128_timer_duration);
        ui->pushButton_timer_start_stop->setDisabled(false);
    }
}

void Clock_Main_Timer_Form::on_pushButton_timer_start_stop_clicked()
{
    if(m_is_clocktimer_start == false)
    {
        m_int128_timer_begin_time = m_get_time.get_tv_time_now();
        m_int128_timer_time_use += (m_get_time.get_tv_time_now() - m_int128_timer_begin_time);

        ui->clock_number_display_form_timer
                ->set_time(m_int128_timer_duration - m_int128_timer_time_use);

        clocktimer_start();
    }
    else
    {
        if(m_is_clocktimer_end == false)
        {
            clocktimer_stop();

            m_int128_timer_time_use += (m_get_time.get_tv_time_now() - m_int128_timer_begin_time);
            ui->clock_number_display_form_timer
                    ->set_time(m_int128_timer_duration - m_int128_timer_time_use);

            m_int128_timer_begin_time = 0;
        }
        else
        {
            clocktimer_stop_remind();
            clocktimer_initialization();
        }
    }
}

void Clock_Main_Timer_Form::on_pushButton_timer_reset_clicked()
{
    if(m_is_clocktimer_reminding == true)
    {
        do_when_clock_timer_stop_remind();
    }
    clocktimer_initialization();
}

void Clock_Main_Timer_Form::do_when_clock_timer_stop_remind()
{
    m_is_clocktimer_reminding = false;
    cout << "turn off" << endl;
    clocktimer_stop_remind();

}
