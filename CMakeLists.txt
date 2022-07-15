cmake_minimum_required(VERSION 3.23)

project(engine)
set(HEADER_FILES ${CMAKE_CURRENT_SOURCE_DIR})

find_package(OpenGL REQUIRED)
include_directories(${OPENGL_INCLUDE_DIRS})

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/glew/include)
add_library(glew STATIC ${CMAKE_CURRENT_SOURCE_DIR}/glew/src/glew.c)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_BUILD_TYPE Debug)

add_subdirectory(${CMAKE_SOURCE_DIR}/glfw)

# for all our custom classes, we must add them to the executable
file(GLOB_RECURSE SRC_FILES src/*.cpp)
add_executable(engine main.cpp ${SRC_FILES})

target_link_libraries(engine glfw glew ${OPENGL_LIBRARIES})

# includes custom header files
target_include_directories(engine PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/src)
