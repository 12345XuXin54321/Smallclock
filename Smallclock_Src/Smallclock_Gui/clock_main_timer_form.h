#ifndef CLOCK_MAIN_TIMER_FORM_H
#define CLOCK_MAIN_TIMER_FORM_H

#include <string>

#include <QWidget>

#include "../Smallclock_Base/Get_Time.h"
#include "../Smallclock_Base/App_Data_Save.h"
#include "../Smallclock_Base/Music_Player.h"

using namespace std;

namespace Ui {
    class Clock_Main_Timer_Form;
}

class Clock_Main_Timer_Form : public QWidget
{
    Q_OBJECT

public:
    explicit Clock_Main_Timer_Form(QWidget *parent = nullptr);
    ~Clock_Main_Timer_Form();

    struct Timer_Form_Data
    {
        string timer_command_path;
        string timer_message;
        string timer_music_path;
        void write_save_data(App_Data_Save* data_save)
        {
            data_save->write_next_data(timer_command_path);
            data_save->write_next_data(timer_message);
            data_save->write_next_data(timer_music_path);
        }
        void load_save_data(App_Data_Save* data_save)
        {
            timer_command_path = data_save->read_next_data();
            timer_message = data_save->read_next_data();
            timer_music_path = data_save->read_next_data();
        }
    };

    void set_data(Timer_Form_Data data)
    {
        m_timer_form_data = data;
    }

    Timer_Form_Data get_data()
    {
        return m_timer_form_data;
    }

private slots:
    void on_pushButton_timer_set_clicked();

    void on_pushButton_timer_start_stop_clicked();

    void on_pushButton_timer_reset_clicked();

    void do_when_flush_timer_timeout();

private:
    Ui::Clock_Main_Timer_Form *ui;

    Timer_Form_Data m_timer_form_data;

    //usec
    __int128 m_int128_timer_duration = 0;
    __int128 m_int128_timer_time_use = 0;
    __int128 m_int128_timer_begin_time = 0;

    bool m_is_clocktimer_start = false;
    bool m_is_clocktimer_end = false;
    bool m_is_clocktimer_reminding = false;

    Get_Time m_get_time;
    Music_Player m_music_player;

    QTimer *m_timer_flush;

    void clocktimer_initialization();
    void clocktimer_start();
    void clocktimer_stop();
    void clocktimer_end();
    void clocktimer_flush();

    void clocktimer_start_remind();
    void clocktimer_stop_remind();


    void qtimer_start();
    void qtimer_stop();

    void do_when_clock_timer_end();
    void do_when_clock_timer_stop_remind();

};

#endif // CLOCK_MAIN_TIMER_FORM_H
