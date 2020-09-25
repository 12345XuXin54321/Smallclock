FILE(GLOB INCLUDE_FILES "./*.h")
FILE(GLOB SRC_FILES "./*.cpp" "./*.c")

FILE(GLOB_RECURSE QRC_SOURCE_FILES "./icons/*.qrc")
qt5_add_resources(QRC_FILES ${QRC_SOURCE_FILES})

add_executable(Smallclock
  ${INCLUDE_FILES}
  ${SRC_FILES}
  ${QRC_FILES}
)

target_link_libraries(Smallclock PUBLIC Qt5::Widgets)

target_link_libraries(Smallclock PUBLIC Smallclock_Base)
target_link_libraries(Smallclock PUBLIC Smallclock_Gui)
