FILE(GLOB INCLUDE_FILES "./*.h")
FILE(GLOB SRC_FILES "./*.cpp" "./*.c")

find_package(Qt5 COMPONENTS Widgets REQUIRED)

add_executable(Smallclock
  ${INCLUDE_FILES}
  ${SRC_FILES}
)

target_link_libraries(Smallclock PRIVATE Qt5::Widgets)

target_link_libraries(Smallclock PRIVATE Smallclock_Base)
target_link_libraries(Smallclock PRIVATE Smallclock_Gui)
