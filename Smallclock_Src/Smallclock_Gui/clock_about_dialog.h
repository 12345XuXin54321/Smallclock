#ifndef CLOCK_ABOUT_DIALOG_H
#define CLOCK_ABOUT_DIALOG_H

#include <QDialog>

namespace Ui {
    class Clock_About_Dialog;
}

class Clock_About_Dialog : public QDialog
{
    Q_OBJECT

public:
    explicit Clock_About_Dialog(QWidget *parent = nullptr);
    ~Clock_About_Dialog();

private:
    Ui::Clock_About_Dialog *ui;
};

#endif // CLOCK_ABOUT_DIALOG_H
