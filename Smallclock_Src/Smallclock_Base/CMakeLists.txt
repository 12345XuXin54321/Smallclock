project(Smallclock_Base)

set(THREADS_PREFER_PTHREAD_FLAG ON)

find_package(Threads REQUIRED)

FILE(GLOB INCLUDE_FILES "./*.h")
FILE(GLOB SRC_FILES "./*.cpp" "./*.c")

add_library(Smallclock_Base STATIC
  ${INCLUDE_FILES}
  ${SRC_FILES}
)

target_link_libraries(Smallclock_Base PUBLIC Threads::Threads)
