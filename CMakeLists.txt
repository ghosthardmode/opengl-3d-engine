cmake_minimum_required(VERSION 3.31)
project(main)

SET(target test.c)
#set(CMAKE_VERBOSE_MAKEFILE ON)
#set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -mconsole")
set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
#for windows
#include("C:\\Program Files\\CMake\\share\\cmake-3.31\\Modules\\FindGLFW3.cmake")
find_package(GLFW3 REQUIRED)
include_directories(${GLFW3_INCLUDE_DIR})

add_executable(${CMAKE_PROJECT_NAME} ${target} glad.c mesh.c mesh.h)
target_link_libraries(${CMAKE_PROJECT_NAME} ${GLFW3_LIBRARY})
