#ifndef CLOCK_MAIN_STOPWATCH_FORM_H
#define CLOCK_MAIN_STOPWATCH_FORM_H

#include <string>

#include <QWidget>

#include "../Smallclock_Base/Get_Time.h"

namespace Ui {
    class Clock_Main_Stopwatch_Form;
}

class Clock_Main_Stopwatch_Form : public QWidget
{
    Q_OBJECT

public:
    explicit Clock_Main_Stopwatch_Form(QWidget *parent = nullptr);
    ~Clock_Main_Stopwatch_Form();

private slots:
    void do_when_flush_timer_timeout();

    void on_pushButton_stopwatch_start_stop_clicked();

    void on_pushButton_stopwatch_reset_clicked();

private:
    Ui::Clock_Main_Stopwatch_Form *ui;

    QTimer *m_timer_flush;

    //usec
    __int128 m_int128_stopwatch_time_use = 0;
    __int128 m_int128_stopwatch_begin_time = 0;

    Get_Time m_get_time;

    bool m_is_stopwatch_start = false;

    void qtimer_start();
    void qtimer_stop();

    void stopwatch_start();
    void stopwatch_stop();
    void stopwatch_flush();
};

#endif // CLOCK_MAIN_STOPWATCH_FORM_H
