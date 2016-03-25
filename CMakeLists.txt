cmake_minimum_required(VERSION 3.3)
project(opengl)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

set(SOURCE_FILES main.cpp shader.cpp shader.hpp texture.cpp texture.hpp quad.cpp quad.hpp map.cpp map.hpp)
add_executable(opengl ${SOURCE_FILES})

find_package(PkgConfig REQUIRED)

set(EXECUTABLE opengl)

pkg_search_module(GLFW REQUIRED glfw3)
include_directories(${GLFW_INCLUDE_DIRS})
target_link_libraries(${EXECUTABLE} ${GLFW_STATIC_LIBRARIES})

find_library(GLEW NAMES libGLEW.a)
target_link_libraries(${EXECUTABLE} ${GLEW})
message("${GLEW}")

find_library(SOIL NAMES libSOIL.a)
target_link_libraries(${EXECUTABLE} ${SOIL})
message("${SOIL}")

if (APPLE)
    target_link_libraries(${EXECUTABLE} "-framework OpenGL")
    target_link_libraries(${EXECUTABLE} "-framework CoreFoundation")
else ()
    find_package(OpenGL REQUIRED)
    include_directories(${OPENGL_INCLUDE_DIRS})
    target_link_libraries(${EXECUTABLE} ${OPENGL_LIBRARIES})
endif ()

