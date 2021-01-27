project(Smallclock_Gui)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

# QtCreator supports the following variables for Android, which are identical to qmake Android variables.
# Check http://doc.qt.io/qt-5/deployment-android.html for more information.
# They need to be set before the find_package(Qt5 ...) call.

#if(ANDROID)
#    set(ANDROID_PACKAGE_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/android")
#    if (ANDROID_ABI STREQUAL "armeabi-v7a")
#        set(ANDROID_EXTRA_LIBS
#            ${CMAKE_CURRENT_SOURCE_DIR}/path/to/libcrypto.so
#            ${CMAKE_CURRENT_SOURCE_DIR}/path/to/libssl.so)
#    endif()
#endif()

find_package(Qt5 COMPONENTS Widgets REQUIRED)

find_package(PkgConfig REQUIRED)

# libnotify start
pkg_check_modules(LIB_NOTIFY libnotify)
include_directories(${LIB_NOTIFY_INCLUDE_DIRS})
# libnotify end

FILE(GLOB INCLUDE_FILES "./*.h")
FILE(GLOB SRC_FILES "./*.cpp" "./*.c")
FILE(GLOB UI_FILES "./*.ui")

add_library(Smallclock_Gui STATIC
  ${INCLUDE_FILES}
  ${SRC_FILES}
  ${UI_FILES}
)

target_link_libraries(Smallclock_Gui PUBLIC Qt5::Widgets)
target_link_libraries(Smallclock_Gui PUBLIC notify)
