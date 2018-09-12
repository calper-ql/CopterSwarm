cmake_minimum_required(VERSION 3.5)
project(HiveEngineRender)

set(CMAKE_CXX_FLAGS "-Wall")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++14")
#set(CMAKE_CXX_FLAGS_RELEASE "-Ofast -fPIC")
set(CMAKE_C_STANDARD 11)

set(SOURCES Camera.cc Camera.h TextRenderer.cc TextRenderer.h GLHandler.cc GLHandler.h ShaderLoader.cc ShaderLoader.h ShaderProgram.cc ShaderProgram.h opengl_settings.h LineRenderer.cc LineRenderer.h glad/glad.c glad/glad.h file_loader.cc file_loader.h)

if (WIN32)
    message(${CMAKE_CURRENT_SOURCE_DIR}/../glfw/include)
    include_directories(${CMAKE_CURRENT_SOURCE_DIR}/../glfw/include)
else ()
    find_package(glfw3 REQUIRED)
endif ()

message(STATUS "CMAKE_CXX_FLAGS renderer: " ${CMAKE_CXX_FLAGS})

find_package(OpenGL REQUIRED)
include_directories(${OPENGL_INCLUDE_DIRS})
set(LIBS glfw ${OPENGL_LIBRARIES})
add_library(HiveEngineRenderer STATIC ${SOURCES})

if (WIN32)

else ()
    target_link_libraries(HiveEngineRenderer ${LIBS})
    target_link_libraries(HiveEngineRenderer ${CMAKE_DL_LIBS})
endif ()

file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/GL_CODE DESTINATION ${CMAKE_BINARY_DIR})
message(STATUS "COPIED GL SRC")
message(STATUS "GL INCLUDE: " ${OPENGL_INCLUDE_DIRS})
