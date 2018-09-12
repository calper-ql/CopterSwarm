cmake_minimum_required(VERSION 3.5)
project(CopterSwarm)

set(CMAKE_CXX_FLAGS "-Wall")
set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 14)
add_subdirectory(HiveEngine)

message(STATUS "RELEASE FLAGS: " ${CMAKE_CXX_FLAGS_RELEASE})
message(STATUS "DEBUG FLAGS: " ${CMAKE_CXX_FLAGS_DEBUG})

add_executable(test_v1 test_v1.cc)
target_link_libraries(test_v1 HiveEngine)

# ANY EXECUTABLE USING OPENGL SHOULD BE BUILT USING THIS CHECK STATEMENT
option(BUILD_WITH_GL "Build the tests with OpenGL to visualize" ON)
if (BUILD_WITH_GL)
    add_subdirectory(HiveEngineRenderer)

    if (WIN32)
        message(${CMAKE_CURRENT_SOURCE_DIR}/glfw/include)
        include_directories(${CMAKE_CURRENT_SOURCE_DIR}/glfw/include)
    endif ()

    # EXCECUTABLE EXAMPLE
    add_executable(test_v2 test_v2.cc)
    target_link_libraries(test_v2 HiveEngine HiveEngineRenderer)

    if (WIN32)
        #target_link_libraries(test_v2 ${CMAKE_CURRENT_SOURCE_DIR}/glfw/lib-mingw-w64/libglfw3dll.a)
        target_link_libraries(test_v2 ${CMAKE_CURRENT_SOURCE_DIR}/glfw/lib-mingw-w64/libglfw3.a)
    endif ()
endif ()

