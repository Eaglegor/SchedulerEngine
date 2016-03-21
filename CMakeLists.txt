cmake_minimum_required(VERSION 2.8)
project(CPP_SCHEDULER)

if(NOT MSVC)
	set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -Wno-attributes ")
else()
	add_definitions( "/wd4251 /wd4275" )
endif()

set(CPP_SCHEDULER_BUILD_ENABLE_CONAN TRUE CACHE BOOL "If enabled, precompiled headers and unity builds will be enabled (provided by Cotire)")
set(CPP_SCHEDULER_BUILD_ENABLE_COTIRE FALSE CACHE BOOL "If enabled, precompiled headers and unity builds will be enabled (provided by Cotire)")
set(CPP_SCHEDULER_BENCHMARK_DATA_DIRECTORY CACHE PATH "Path to the benchmark data directory")

set(CPP_SCHEDULER_ENGINE TRUE CACHE BOOL "If enabled, engine will be built")
set(CPP_SCHEDULER_PERSISTENCE TRUE CACHE BOOL "If enabled, loaders and persisters will be built")
set(CPP_SCHEDULER_SERVICES TRUE CACHE BOOL "If enabled, services like routing will be built")
set(CPP_SCHEDULER_TOOLS TRUE CACHE BOOL "If enabled, utilitary tools will be built")
set(CPP_SCHEDULER_TESTS FALSE CACHE BOOL "If enabled, tests will be built")

set(__CONAN_ARGS "install")

set(__CONAN_ARGS "${__CONAN_ARGS} --build=missing")

cmake_policy(SET CMP0054 OLD)

if (${CMAKE_CXX_COMPILER_ID} STREQUAL "MSVC")
  message(STATUS "Configuring conan for MSVC")
  set(__CONAN_ARGS "${__CONAN_ARGS} -s compiler=\"Visual Studio\"")
elseif(${CMAKE_CXX_COMPILER_ID} STREQUAL "GNU")
  message(STATUS "Configuring conan for GCC")
  set(__CONAN_ARGS "${__CONAN_ARGS} -s compiler=gcc -s compiler.libcxx=libstdc++11")
elseif(${CMAKE_CXX_COMPILER_ID} STREQUAL "Clang")
  message(STATUS "Configuring conan for Clang")
  set(__CONAN_ARGS "${__CONAN_ARGS} -s compiler=clang  -s compiler.libcxx=libstdc++11") 
else()
  message(STATUS "Unknown compiler")
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

if(CPP_SCHEDULER_BUILD_ENABLE_CONAN)
    execute_process(COMMAND conan ${TESTVAR} WORKING_DIRECTORY ${CPP_SCHEDULER_SOURCE_DIR})
    
    include(conanbuildinfo.cmake)

    conan_basic_setup()
endif()

set_property(GLOBAL PROPERTY USE_FOLDERS ON)

if(CPP_SCHEDULER_TESTS)
    enable_testing()
endif()

include_directories(${CPP_SCHEDULER_SOURCE_DIR})
include_directories(${CPP_SCHEDULER_BINARY_DIR}/SharedLibrariesExportHeaders/)
include_directories(${CPP_SCHEDULER_SOURCE_DIR}/Tests/IncludeGroups)

set(EXECUTABLE_OUTPUT_PATH ${CPP_SCHEDULER_BINARY_DIR}/bin)
set(LIBRARY_OUTPUT_PATH ${CPP_SCHEDULER_BINARY_DIR}/lib)

if(CPP_SCHEDULER_BUILD_ENABLE_COTIRE)
    include(cmake/cotire/cotire)
else()
    macro(cotire)
    endmacro()
endif()

if(CPP_SCHEDULER_ENGINE)
    add_subdirectory(Engine)
endif()

if(CPP_SCHEDULER_SERVICES)
    add_subdirectory(Services)
endif()

if(CPP_SCHEDULER_PERSISTENCE)
    add_subdirectory(Persistence)
endif()

if(CPP_SCHEDULER_TOOLS)
    add_subdirectory(Tools)
endif()

if(CPP_SCHEDULER_TESTS)
    add_subdirectory(Tests)
endif()