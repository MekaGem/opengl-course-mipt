cmake_minimum_required(VERSION 3.3)
project(299OsipovAndrey)

set(EXECUTABLE 299OsipovAndrey)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
set(SOURCE_FILES main.cpp shader.cpp shader.hpp texture.cpp texture.hpp quad.cpp quad.hpp map.cpp map.hpp)
set(HEADER_FILES shader.hpp texture.hpp quad.hpp map.hpp)

add_executable(${EXECUTABLE} ${SOURCE_FILES} ${HEADER_FILES})

if (DEPENDENCIES_ROOT)
    set(DATA_ROOT "${EXECUTABLE}Data")

    include(${DEPENDENCIES_ROOT}/lib/cmake/glew/glew-targets.cmake)
    include(${DEPENDENCIES_ROOT}/lib/cmake/glfw/glfwTargets.cmake)
    include(${DEPENDENCIES_ROOT}/lib/cmake/glm/glmTargets.cmake OPTIONAL)
    include(${DEPENDENCIES_ROOT}/lib64/cmake/glm/glmTargets.cmake OPTIONAL)
    include(${DEPENDENCIES_ROOT}/lib/cmake/assimp-3.2/assimpTargets.cmake)
    include(${DEPENDENCIES_ROOT}/lib/cmake/soil/soilTargets.cmake)
else ()
    set(DATA_ROOT ".")

    find_package(PkgConfig REQUIRED)

    pkg_search_module(GLFW REQUIRED glfw3)
    include_directories(${GLFW_INCLUDE_DIRS})
    target_link_libraries(${EXECUTABLE} ${GLFW_STATIC_LIBRARIES})

    find_library(GLEW NAMES libGLEW.a)
    target_link_libraries(${EXECUTABLE} ${GLEW})

    find_library(SOIL NAMES libSOIL.a)
    target_link_libraries(${EXECUTABLE} ${SOIL})

    if (APPLE)
        target_link_libraries(${EXECUTABLE} "-framework OpenGL")
        target_link_libraries(${EXECUTABLE} "-framework Cocoa")
        target_link_libraries(${EXECUTABLE} "-framework CoreVideo")
        target_link_libraries(${EXECUTABLE} "-framework IOKit")
        target_link_libraries(${EXECUTABLE} "-framework Carbon")
    else ()
        find_package(OpenGL REQUIRED)
        include_directories(${OPENGL_INCLUDE_DIRS})
        target_link_libraries(${EXECUTABLE} ${OPENGL_LIBRARIES})
    endif ()
endif ()

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DDATA_ROOT='\"${DATA_ROOT}\"'")

install(TARGETS ${EXECUTABLE} RUNTIME DESTINATION ${CMAKE_INSTALL_PREFIX})
install(DIRECTORY data DESTINATION "${CMAKE_INSTALL_PREFIX}/${DATA_ROOT}")