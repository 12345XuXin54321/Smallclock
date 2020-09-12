#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <getopt.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>

#include <libnotify/notify.h>

#include "Message_Window_Show.h"

using namespace std;

void Message_Window_Show::showMessageWindow(const char* app_name, const char* summary,
                                            const char* message_body, const char* icon_path)
{
    notify_init(app_name);
    NotifyNotification *notify = notify_notification_new(summary,
                                                         message_body,
                                                         icon_path);
    notify_notification_show(notify, NULL);
}

