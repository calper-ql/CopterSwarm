cmake_minimum_required(VERSION 3.5)
project(CopterSwarm)

set(CMAKE_CXX_FLAGS "-Wall")
set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 14)
add_subdirectory(HiveEngine)
add_subdirectory(CopterLib)

message(STATUS "RELEASE FLAGS: " ${CMAKE_CXX_FLAGS_RELEASE})
message(STATUS "DEBUG FLAGS: " ${CMAKE_CXX_FLAGS_DEBUG})

add_executable(test_v1 test_v1.cc)
target_link_libraries(test_v1 HiveEngine CopterLib)

if(MINGW)
    add_executable(environment_server win32_environment_server.cc)
    target_link_libraries(environment_server HiveEngine CopterLib)

    add_executable(environment_server_test_client win32_environment_server_test_client.cc)
    target_link_libraries(environment_server_test_client HiveEngine CopterLib)

    target_link_libraries(environment_server ws2_32 mswsock advapi32)
    target_link_libraries(environment_server_test_client ws2_32)
else()

endif()

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
    target_link_libraries(test_v2 HiveEngine HiveEngineRenderer CopterLib)

    add_executable(test_v3 test_v3.cc)
    target_link_libraries(test_v3 HiveEngine HiveEngineRenderer CopterLib)

    add_executable(test_v4 test_v4.cc)
    target_link_libraries(test_v4 HiveEngine HiveEngineRenderer CopterLib)

    if (WIN32)
        target_link_libraries(test_v2 ${CMAKE_CURRENT_SOURCE_DIR}/glfw/lib-mingw-w64/libglfw3.a)
        target_link_libraries(test_v3 ${CMAKE_CURRENT_SOURCE_DIR}/glfw/lib-mingw-w64/libglfw3.a)
        target_link_libraries(test_v4 ${CMAKE_CURRENT_SOURCE_DIR}/glfw/lib-mingw-w64/libglfw3.a)
    endif ()
endif ()

