project(Smallclock_Base)

set(THREADS_PREFER_PTHREAD_FLAG ON)

find_package(Threads REQUIRED)

find_package(PkgConfig REQUIRED)

# libvlc start
pkg_check_modules(LIB_VLC libvlc)
include_directories(${LIB_VLC_INCLUDE_DIRS})
# libvlc end

FILE(GLOB INCLUDE_FILES "./*.h")
FILE(GLOB SRC_FILES "./*.cpp" "./*.c")

add_library(Smallclock_Base STATIC
  ${INCLUDE_FILES}
  ${SRC_FILES}
)

target_link_libraries(Smallclock_Base PUBLIC Threads::Threads)
target_link_libraries(Smallclock_Base PUBLIC vlc)
