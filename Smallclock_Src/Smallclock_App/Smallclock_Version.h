#ifndef SMALLCLOCK_VERSION_H
#define SMALLCLOCK_VERSION_H

#include <string>

#include <QtConfig>
#include <libnotify/notify-features.h>
#include <vlc/libvlc_version.h>

#include "../../config.h"

using namespace std;

const string const_str_Smallclock_version =
        "Smallclock version " +
        to_string(APP_VERSION_MAJOR) +
        "." +
        to_string(APP_VERSION_MINOR) +
        "." +
        to_string(APP_VERSION_REVISION) +
        "-" +
        to_string(APP_BUILD_TIMES);

const string const_str_Smallclock_version_write_in_data =
        "<Smallclock_version=" +
        to_string(APP_VERSION_MAJOR) +
        "." +
        to_string(APP_VERSION_MINOR) +
        "." +
        to_string(APP_VERSION_REVISION) +
        ">";

const string const_str_Smallclock_data_version =
        "<Smallclock_data_version=" +
        to_string(APP_DATA_VERSION_MAJOR) +
        "." +
        to_string(APP_DATA_VERSION_MINOR) +
        ">";

const string const_str_Qt_version =
        string("QT version : ") +
        QT_VERSION_STR;

const string const_str_gcc_version =
        string("gcc version : ") +
        __VERSION__;

const string const_str_libnotify_version =
        "libnotify version " +
        to_string(NOTIFY_VERSION_MAJOR) +
        "." +
        to_string(NOTIFY_VERSION_MINOR) +
        "." +
        to_string(NOTIFY_VERSION_MICRO);

const string const_str_libvlc_version =
        "libvlc version " +
        to_string(LIBVLC_VERSION_MAJOR) +
        "." +
        to_string(LIBVLC_VERSION_MINOR) +
        "." +
        to_string(LIBVLC_VERSION_REVISION);


#endif // SMALLCLOCK_VERSION_H
