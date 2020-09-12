#ifndef CLOCK_NUMBER_DISPLAY_FORM_H
#define CLOCK_NUMBER_DISPLAY_FORM_H

#include <ctime>

#include <QWidget>
#include <QTimer>

namespace Ui {
    class Clock_Number_Display_Form;
}

class Clock_Number_Display_Form : public QWidget
{
    Q_OBJECT

public:
    explicit Clock_Number_Display_Form(QWidget *parent = nullptr);
    ~Clock_Number_Display_Form();

    void set_hour(int hour);
    void set_min(int min);
    void set_sec(int sec);
    void set_msec(int msec);
    void set_time(__int128 usec);

    void start_flickered_remind();
    void stop_flickered_remind();

private slots:
    void do_to_flickered_remind();

private:
    Ui::Clock_Number_Display_Form *ui;

    QTimer *m_flickered_remind_timer;

    void set_style_diaphaneity();
    void set_style(bool value);

    bool is_light = true;
};

#endif // CLOCK_NUMBER_DISPLAY_FORM_H
