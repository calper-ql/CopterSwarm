cmake_minimum_required(VERSION 3.5)
project(CopterLib)

find_package(Threads REQUIRED)

set(CMAKE_CXX_FLAGS "-Wall")
set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 14)

set(SOURCES Rotor.h Rotor.cc Motor.cc Motor.h EnergySource.cc EnergySource.h CommonStructs.h Copter.cc Copter.h Utilities.cc Utilities.h Environment.cc Environment.h Serializable.h)

add_library(CopterLib STATIC ${SOURCES})
target_link_libraries(CopterLib ${CMAKE_THREAD_LIBS_INIT})

