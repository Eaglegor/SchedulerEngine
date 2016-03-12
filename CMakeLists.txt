cmake_minimum_required(VERSION 2.8)
project(CPP_SCHEDULER)

if(NOT MSVC)
	set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
else()
	add_definitions( "/wd4251" )
endif()

set(__CONAN_ARGS "install")

set(__CONAN_ARGS "${__CONAN_ARGS} --build=missing")

if (MSVC)
  set(__CONAN_ARGS "${__CONAN_ARGS} -s compiler=\"Visual Studio\"")
endif()

if(NOT CMAKE_BUILD_TYPE)
	set(CMAKE_BUILD_TYPE Debug)
endif()

if(CMAKE_BUILD_TYPE STREQUAL "Debug")
	set(__CONAN_ARGS "${__CONAN_ARGS} -s build_type=Debug")
	if(MSVC)
		set(__CONAN_ARGS "${__CONAN_ARGS} -s compiler.runtime=MDd")
	endif()
else()
	set(__CONAN_ARGS "${__CONAN_ARGS} -s build_type=Release")
	if(MSVC)
		set(__CONAN_ARGS "${__CONAN_ARGS} -s compiler.runtime=MD")
	endif()
endif()

if(CMAKE_SIZEOF_VOID_P EQUAL 8)
	set(__CONAN_ARGS "${__CONAN_ARGS} -s arch=x86_64")
else()
	set(__CONAN_ARGS "${__CONAN_ARGS} -s arch=x86")
endif()

message(STATUS "Updating dependencies")
separate_arguments(TESTVAR WINDOWS_COMMAND "${__CONAN_ARGS}")
execute_process(COMMAND conan ${TESTVAR} WORKING_DIRECTORY ${CPP_SCHEDULER_SOURCE_DIR})

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
add_subdirectory(Math)
add_subdirectory(Utils)
add_subdirectory(Persistence)