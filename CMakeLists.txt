cmake_minimum_required(VERSION 3.23)

project(engine)

find_package(OpenGL REQUIRED)
include_directories(${OPENGL_INCLUDE_DIRS})

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/glew/include)
add_library(glew STATIC ${CMAKE_CURRENT_SOURCE_DIR}/glew/src/glew.c)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

add_subdirectory(${CMAKE_SOURCE_DIR}/glfw)

add_executable(engine main.cpp)
target_link_libraries(engine glfw glew ${OPENGL_LIBRARIES})
