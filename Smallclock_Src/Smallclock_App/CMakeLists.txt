FILE(GLOB INCLUDE_FILES "./*.h")
FILE(GLOB SRC_FILES "./*.cpp" "./*.c")

FILE(GLOB_RECURSE QRC_SOURCE_FILES "./resources/*.qrc")

qt6_add_resources(QRC_FILES ${QRC_SOURCE_FILES})

# libvlc start
pkg_check_modules(LIB_VLC libvlc)
include_directories(${LIB_VLC_INCLUDE_DIRS})
# libvlc end

# libnotify start
pkg_check_modules(LIB_NOTIFY libnotify)
include_directories(${LIB_NOTIFY_INCLUDE_DIRS})
# libnotify end

add_executable(Smallclock
  ${INCLUDE_FILES}
  ${SRC_FILES}
  ${QRC_FILES}
)

target_link_libraries(Smallclock PUBLIC Smallclock_Base)
target_link_libraries(Smallclock PUBLIC Smallclock_Gui)

target_link_libraries(Smallclock PUBLIC Qt6::Widgets)
target_link_libraries(Smallclock PUBLIC vlc)
target_link_libraries(Smallclock PUBLIC notify)
