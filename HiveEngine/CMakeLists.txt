cmake_minimum_required(VERSION 3.5)
project(HiveEngine)

find_package(Threads REQUIRED)

set(CMAKE_CXX_FLAGS "-Wall")
set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 14)

set(SOURCES Entity.cc Entity.h Utilities.h Utilities.cc)

add_library(HiveEngine STATIC ${SOURCES})
target_link_libraries(HiveEngine ${CMAKE_THREAD_LIBS_INIT})

