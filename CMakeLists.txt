cmake_minimum_required(VERSION 2.8)
project(CPP_SCHEDULER_NG)

set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

enable_testing()

include_directories(${CPP_SCHEDULER_NG_SOURCE_DIR})

add_subdirectory(Engine)
add_subdirectory(Tests)
add_subdirectory(Plugins)