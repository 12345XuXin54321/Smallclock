#ifndef CLOCKTIMER_CHOOSE_TIME_DIALOG_H
#define CLOCKTIMER_CHOOSE_TIME_DIALOG_H

#include <QDialog>

namespace Ui {
    class ClockTimer_Choose_Time_Dialog;
}

class ClockTimer_Choose_Time_Dialog : public QDialog
{
    Q_OBJECT

public:
    explicit ClockTimer_Choose_Time_Dialog(QWidget *parent = nullptr);
    ~ClockTimer_Choose_Time_Dialog();

    __int128 get_hour();
    __int128 get_min();
    __int128 get_sec();

private:
    Ui::ClockTimer_Choose_Time_Dialog *ui;
};

#endif // CLOCKTIMER_CHOOSE_TIME_DIALOG_H
