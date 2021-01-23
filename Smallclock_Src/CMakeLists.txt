set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

find_package(Qt6 COMPONENTS Widgets REQUIRED)

add_subdirectory(Smallclock_App)
add_subdirectory(Smallclock_Base)
add_subdirectory(Smallclock_Gui)

include_directories(Smallclock_App)
include_directories(Smallclock_Base)
include_directories(Smallclock_Gui)
