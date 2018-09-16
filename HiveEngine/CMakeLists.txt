cmake_minimum_required(VERSION 3.5)
project(HiveEngine)

find_package(Threads REQUIRED)

set(CMAKE_CXX_FLAGS "-Wall")
set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 14)

set(SOURCES Entity_Deprecated.cc Entity_Deprecated.h Utilities.h Utilities.cc Entity.cc Entity.h CommonStructs.h CommonStructs.cc PhysicalNode.cc PhysicalNode.h)

add_library(HiveEngine STATIC ${SOURCES})
target_link_libraries(HiveEngine ${CMAKE_THREAD_LIBS_INIT})

