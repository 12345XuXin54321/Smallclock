FILE(GLOB INCLUDE_FILES "./*.h")
FILE(GLOB SRC_FILES "./*.cpp" "./*.c")

FILE(GLOB_RECURSE QRC_SOURCE_FILES "./resources/*.qrc")

#FILE(GLOB QT_STATIC_PLUGINS_LIBS "${CMAKE_PREFIX_PATH}/plugins/*/*.a")

qt6_add_resources(QRC_FILES ${QRC_SOURCE_FILES})

add_executable(Smallclock
  ${INCLUDE_FILES}
  ${SRC_FILES}
  ${QRC_FILES}
)

target_link_libraries(Smallclock PUBLIC Smallclock_Base)
target_link_libraries(Smallclock PUBLIC Smallclock_Gui)

target_link_libraries(Smallclock PUBLIC Qt6::Widgets)
#target_link_libraries(Smallclock PUBLIC ${QT_STATIC_PLUGINS_LIBS})
