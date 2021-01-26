#include <iostream>

#include <QFile>

#include "../Smallclock_App/Smallclock_Version.h"

#include "clock_about_dialog.h"
#include "ui_clock_about_dialog.h"

using namespace std;

Clock_About_Dialog::Clock_About_Dialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Clock_About_Dialog)
{
    ui->setupUi(this);

    ui->m_title_lineEdit->setText(QString::fromStdString(const_str_Smallclock_version));
    ui->m_buildTime_lineEdit->setText("built on " +
                                      QString(__DATE__) +
                                      " " +
                                      QString(__TIME__));

    QFile readme_file(":/program_documents/SmallClock_Readme.txt");
    readme_file.open(QFile::OpenModeFlag::ReadOnly);
    ui->m_expository_textEdit->
            setText(QString(readme_file.readAll()));

    string str_base_on = const_str_gcc_version + "\n" +
            const_str_Qt_version + "\n" +
            const_str_libnotify_version + "\n" +
            const_str_libvlc_version;
    ui->m_base_on_textEdit->setText(QString(str_base_on.c_str()));

}

Clock_About_Dialog::~Clock_About_Dialog()
{
    delete ui;
}
