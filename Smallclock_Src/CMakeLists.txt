set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(THREADS_PREFER_PTHREAD_FLAG ON)

find_package(Qt5 COMPONENTS Widgets REQUIRED)
find_package(Threads REQUIRED)

find_package(PkgConfig REQUIRED)

# libnotify start
pkg_check_modules(LIB_NOTIFY_PKG libnotify)
include_directories(${LIB_NOTIFY_PKG_INCLUDE_DIRS})
# libnotify end

add_subdirectory(Smallclock_App)
add_subdirectory(Smallclock_Base)
add_subdirectory(Smallclock_Gui)

include_directories(Smallclock_App)
include_directories(Smallclock_Base)
include_directories(Smallclock_Gui)
