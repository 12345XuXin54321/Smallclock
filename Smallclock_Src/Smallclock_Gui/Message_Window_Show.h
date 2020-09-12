#ifndef MESSAGE_WINDOW_SHOW_H
#define MESSAGE_WINDOW_SHOW_H

#include <iostream>

using namespace std;

class Message_Window_Show
{
public:
    static void showMessageWindow(const char* app_name, const char* summary,
                                  const char* message_body, const char* icon_path);
};

#endif // MESSAGE_WINDOW_SHOW_H
