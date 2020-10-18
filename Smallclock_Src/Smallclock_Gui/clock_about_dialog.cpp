#include <iostream>

#include <QFile>

#include "../../Smallclock_version.h"

#include "clock_about_dialog.h"
#include "ui_clock_about_dialog.h"

using namespace std;

const string m_const_str_Smallclock_version =
        "Smallclock " +
        to_string(Smallcllock_version_major) +
        "." +
        to_string(Smallcllock_version_minor) +
        "." +
        to_string(Smallcllock_version_revision);

Clock_About_Dialog::Clock_About_Dialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Clock_About_Dialog)
{
    ui->setupUi(this);

    ui->m_title_lineEdit->setText(QString::fromStdString(m_const_str_Smallclock_version));
    ui->m_buildTime_lineEdit->setText("built on " +
                                      QString(__DATE__) +
                                      " " +
                                      QString(__TIME__));

    QFile readme_file(":/program_documents/SmallClock_Readme.txt");
    readme_file.open(QFile::OpenModeFlag::ReadOnly);
    ui->m_expository_textEdit->
            setText(QString(readme_file.readAll()));
}

Clock_About_Dialog::~Clock_About_Dialog()
{
    delete ui;
}
