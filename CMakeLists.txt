cmake_minimum_required(VERSION 3.3)
project(opengl)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

set(SOURCE_FILES main.cpp shader.cpp shader.hpp texture.cpp texture.hpp quad.cpp quad.hpp)
add_executable(opengl ${SOURCE_FILES})

find_package(PkgConfig REQUIRED)

pkg_search_module(GLFW REQUIRED glfw3)
include_directories(${GLFW_INCLUDE_DIRS})
target_link_libraries(opengl ${GLFW_STATIC_LIBRARIES})

find_library(GLEW NAMES libGLEW.a)
target_link_libraries(opengl ${GLEW})
message("${GLEW}")

find_library(SOIL NAMES libSOIL.a)
target_link_libraries(opengl ${SOIL})
message("${SOIL}")

if (APPLE)
    target_link_libraries(opengl "-framework OpenGL")
    target_link_libraries(opengl "-framework CoreFoundation")
endif()

