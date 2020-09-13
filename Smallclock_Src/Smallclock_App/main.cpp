#include <QApplication>

#include "../Smallclock_Gui/clock_main_window.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Clock_Main_Window w;
    return a.exec();
}
