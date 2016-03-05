cmake_minimum_required(VERSION 2.8)
project(CPP_SCHEDULER)

if(NOT MSVC)
	set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
endif()

set(__CONAN_COMPILER_OPTIONS "--build=missing")

if (MSVC)
  set(__CONAN_COMPILER_OPTIONS "${__CONAN_COMPILER_OPTIONS} -s compiler=\"Visual Studio\"")
endif()

if(${CMAKE_SIZEOF_VOID_P} EQUAL 8)
	set(__CONAN_COMPILER_OPTIONS "${__CONAN_COMPILER_OPTIONS} -s arch=x86")
else()
	set(__CONAN_COMPILER_OPTIONS "${__CONAN_COMPILER_OPTIONS} -s arch=x86_64")
endif()

message(STATUS "Updating dependencies")
execute_process(COMMAND conan install ${__CONAN_COMPILER_OPTIONS} WORKING_DIRECTORY ${CPP_SCHEDULER_SOURCE_DIR} RESULT_VARIABLE rv)

include(conanbuildinfo.cmake)

conan_basic_setup()

set_property(GLOBAL PROPERTY USE_FOLDERS ON)

enable_testing()

include_directories(${CPP_SCHEDULER_SOURCE_DIR})
include_directories(${CPP_SCHEDULER_BINARY_DIR}/SharedLibrariesExportHeaders/)

set(EXECUTABLE_OUTPUT_PATH ${CPP_SCHEDULER_BINARY_DIR}/bin)
set(LIBRARY_OUTPUT_PATH ${CPP_SCHEDULER_BINARY_DIR}/lib)

add_subdirectory(Engine)
add_subdirectory(Tests)
add_subdirectory(Plugins)