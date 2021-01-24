#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <getopt.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>

#include <libnotify/notify.h>

#include "Message_Window_Show.h"
#include "../../config.h"

using namespace std;

void Message_Window_Show::showMessageWindow(const char* summary,
                                            const char* message_body)
{
    notify_init(APP_NAME);
    NotifyNotification *notify = notify_notification_new(summary,
                                                         message_body,
                                                         ":/program_icons/SmallClock.png");
    notify_notification_show(notify, NULL);
}

