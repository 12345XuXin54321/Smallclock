#include "clock_main_alarmclock_form.h"
#include "ui_clock_main_alarmclock_form.h"

Clock_Main_AlarmClock_Form::Clock_Main_AlarmClock_Form(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Clock_Main_AlarmClock_Form)
{
    ui->setupUi(this);

    ui->treeWidget_alarm_clock->setContextMenuPolicy(Qt::ContextMenuPolicy::CustomContextMenu);
    ui->treeWidget_alarm_clock->
            connect(ui->treeWidget_alarm_clock,
                    SIGNAL(customContextMenuRequested(const QPoint&)),
                    this,
                    SLOT(alarmClockTreeWidgetItem_rightKey_menu_clicked(const QPoint&)));

    m_timer_alarm_clock = new QTimer(this);
    connect(m_timer_alarm_clock, SIGNAL(timeout()), this, SLOT(do_when_alarm_clock_timer_timeout()));
    m_timer_alarm_clock->start(800);
}

Clock_Main_AlarmClock_Form::~Clock_Main_AlarmClock_Form()
{
    delete ui;
}

void Clock_Main_AlarmClock_Form::set_data(AlarmClock_Form_Data data)
{
    m_alarm_clock_default_setting = data.alarm_clock_default_setting;

    ui->treeWidget_alarm_clock->clear();
    for(auto alarmClock : data.list_alarm_clock_using)
    {
        alarmClock_add(alarmClock);
    }

}

Clock_Main_AlarmClock_Form::AlarmClock_Form_Data Clock_Main_AlarmClock_Form::get_data()
{
    AlarmClock_Form_Data alarm_clock_data;

    alarm_clock_data.alarm_clock_default_setting = m_alarm_clock_default_setting;

    QTreeWidgetItemIterator itor_alarm_clock(ui->treeWidget_alarm_clock);
    while(*itor_alarm_clock)
    {
        alarm_clock_data.list_alarm_clock_using.push_back(
                    ((alarmClock_set*)(*itor_alarm_clock))->get_alarmClock()
                    );

        (*itor_alarm_clock)->text(0);//?????
        ++itor_alarm_clock;
    }

    return alarm_clock_data;
}

void Clock_Main_AlarmClock_Form::alarmClock_add(Alarm_Clock new_alarm_clock)
{
    alarmClock_set* alarm_clock_set = new alarmClock_set(new_alarm_clock);
    ui->treeWidget_alarm_clock->addTopLevelItem(alarm_clock_set);
}
void Clock_Main_AlarmClock_Form::alarmClock_edit(Alarm_Clock new_alarm_clock,
                                        alarmClock_set* alarm_clock_set)
{
    alarm_clock_set->edit_alarm_clock(new_alarm_clock);
}
void Clock_Main_AlarmClock_Form::alarmClock_delete(alarmClock_set *alarm_clock_set)
{
    int index = ui->treeWidget_alarm_clock->
            indexOfTopLevelItem(alarm_clock_set);
    ui->treeWidget_alarm_clock->takeTopLevelItem(index);
}

bool Clock_Main_AlarmClock_Form::compare_alarm_clock_day_time_and_now(Alarm_Clock alarm_clock)
{
    QTime now_time = QTime::currentTime();
    if(now_time.hour() == alarm_clock.m_alarm_clock_data.int_hour &&
            now_time.minute() == alarm_clock.m_alarm_clock_data.int_min)
    {
        return true;
    }
    else
    {
        return false;
    }
}
bool Clock_Main_AlarmClock_Form::compare_alarm_clock_week_day_and_now(Alarm_Clock alarm_clock)
{
    QDateTime now_date_time = QDateTime::currentDateTime();
    cout << now_date_time.date().dayOfWeek() << endl;
    for(auto day_in_week : alarm_clock.m_alarm_clock_data.vector_alarm_time_range_a_week)
    {
        if(now_date_time.date().dayOfWeek() == day_in_week)
        {
            return true;
        }
    }
    return false;
}
bool Clock_Main_AlarmClock_Form::compare_alarm_clock_month_and_now(Alarm_Clock alarm_clock)
{
    QDate now_date = QDate::currentDate();
    if(now_date.month() == alarm_clock.m_alarm_clock_data.int_month)
    {
        return true;
    }
    else
    {
        return false;
    }
}
bool Clock_Main_AlarmClock_Form::compare_alarm_clock_dateTime_and_now(Alarm_Clock alarm_clock)
{
    QDateTime now_date_time = QDateTime::currentDateTime();
    QDateTime alarm_clock_date_time = QDateTime::fromSecsSinceEpoch(alarm_clock.m_alarm_clock_data.longlongint_accurate_time);
    if(now_date_time.date().year() == alarm_clock_date_time.date().year() &&
            now_date_time.date().dayOfYear() == alarm_clock_date_time.date().dayOfYear() &&
            now_date_time.time().hour() == alarm_clock_date_time.time().hour() &&
            now_date_time.time().minute() == alarm_clock_date_time.time().minute())
    {
        return true;
    }
    else
    {
        return false;
    }
}

void Clock_Main_AlarmClock_Form::pushButton_alarmClock_add_new_clicked()
{
    New_Alarm_Clock_Dialog *new_alarm_clock_dialog = new New_Alarm_Clock_Dialog();
    new_alarm_clock_dialog->set_alarm_clock(m_alarm_clock_default_setting);
    if(new_alarm_clock_dialog->exec() == 1)
    {
        alarmClock_add(new_alarm_clock_dialog->get_alarm_clock_setting());
    }
}
void Clock_Main_AlarmClock_Form::pushButton_alarmClock_edit_clicked()
{
    New_Alarm_Clock_Dialog *new_alarm_clock_dialog = new New_Alarm_Clock_Dialog();
    new_alarm_clock_dialog->set_alarm_clock(m_alarm_clock_item_last_clicked->get_alarmClock());
    if(new_alarm_clock_dialog->exec() == 1)
    {
        alarmClock_edit(new_alarm_clock_dialog->get_alarm_clock_setting(),
                        m_alarm_clock_item_last_clicked);
    }
}
void Clock_Main_AlarmClock_Form::pushButton_alarmClock_delete_clicked()
{
    m_alarm_clock_item_last_clicked->stop_remind();
    alarmClock_delete(m_alarm_clock_item_last_clicked);
}

void Clock_Main_AlarmClock_Form::do_when_alarm_clock_timer_timeout()
{
    QTreeWidgetItemIterator itor(ui->treeWidget_alarm_clock);
    while(*itor)
    {
        alarmClock_set* alarm_clock_set = (alarmClock_set*)(*itor);
        alarm_clock_set->updata_alarmClock_date();
        Alarm_Clock alarm_clock = alarm_clock_set->get_alarmClock();

        if(alarm_clock.m_alarm_clock_data.is_turn_on == true &&
                alarm_clock_set->get_alarm_clock_is_remind() == false)
        {
            switch(alarm_clock.m_alarm_clock_data.timeType_choose_range)
            {
                case Alarm_Clock::Time_Type::range_a_day:
                    {
                        if(compare_alarm_clock_day_time_and_now(alarm_clock) == true)
                        {
                            alarm_clock_set->start_remind();
                        }
                        break;
                    }
                case Alarm_Clock::Time_Type::range_a_week:
                    {
                        if(compare_alarm_clock_day_time_and_now(alarm_clock) == true &&
                                compare_alarm_clock_week_day_and_now(alarm_clock) == true)
                        {
                            alarm_clock_set->start_remind();
                        }
                        break;
                    }
                case Alarm_Clock::Time_Type::range_a_year:
                    {
                        if(compare_alarm_clock_day_time_and_now(alarm_clock) == true &&
                                compare_alarm_clock_month_and_now(alarm_clock) == true)
                        {
                            alarm_clock_set->start_remind();
                        }
                        break;
                    }
                case Alarm_Clock::Time_Type::no_range:
                    {
                        if(compare_alarm_clock_dateTime_and_now(alarm_clock) == true)
                        {
                            alarm_clock_set->start_remind();
                        }
                        break;
                    }
            }
        }

        (*itor)->text(0);
        ++itor;
    }
}

void Clock_Main_AlarmClock_Form::alarmClockTreeWidgetItem_rightKey_menu_clicked(const QPoint& point)
{
    m_alarm_clock_item_last_clicked = (alarmClock_set*)(ui->treeWidget_alarm_clock->itemAt(point));
    QMenu* menu_treeWidget = new QMenu();
    if(m_alarm_clock_item_last_clicked == 0)
    {
        menu_treeWidget->addAction("新建闹钟", this, SLOT(pushButton_alarmClock_add_new_clicked()));
    }
    else
    {
        if(m_alarm_clock_item_last_clicked->get_alarmClock().m_alarm_clock_data.is_turn_on == true)
        {
            if(m_alarm_clock_item_last_clicked->get_alarm_clock_is_remind() == true)
            {
                menu_treeWidget->addAction("停止提醒", this, SLOT(do_when_alarmClock_to_be_stop_remind()));
            }
            else
            {
                menu_treeWidget->addAction("关闭", this, SLOT(do_when_alarmClock_to_be_start_or_stop()));
            }
        }
        else
        {
            menu_treeWidget->addAction("开启", this, SLOT(do_when_alarmClock_to_be_start_or_stop()));
        }
        menu_treeWidget->addSeparator();
        menu_treeWidget->addAction("编辑闹钟", this, SLOT(pushButton_alarmClock_edit_clicked()));
        menu_treeWidget->addAction("删除闹钟", this, SLOT(pushButton_alarmClock_delete_clicked()));
    }
    menu_treeWidget->exec(QCursor::pos());
}

void Clock_Main_AlarmClock_Form::do_when_alarmClock_to_be_stop_remind()
{
    m_alarm_clock_item_last_clicked->stop_remind();
}
void Clock_Main_AlarmClock_Form::do_when_alarmClock_to_be_start_or_stop()
{
    Alarm_Clock alarm_clock = m_alarm_clock_item_last_clicked->get_alarmClock();
    alarm_clock.m_alarm_clock_data.is_turn_on ^= 1;
    m_alarm_clock_item_last_clicked->stop_remind();
    m_alarm_clock_item_last_clicked->edit_alarm_clock(alarm_clock);
}
