cmake_minimum_required(VERSION 3.10)

project(scheduler C)

file(GLOB SRC_FILES ${CMAKE_SOURCE_DIR}/src/*.c)
add_executable(scheduler ${SRC_FILES} main.c)

target_include_directories(scheduler PRIVATE include/)
target_link_libraries(scheduler PRIVATE rt)
set(EXECUTABLE_OUTPUT_PATH ${CMAKE_BINARY_DIR}/bin/)

