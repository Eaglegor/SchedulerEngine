cmake_minimum_required(VERSION 2.8)
project(CPP_SCHEDULER)

if(NOT MSVC)
	set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -Wno-attributes -pthread -fopenmp")
else()
	add_definitions( "/wd4251 /wd4275 /openmp" )
endif()

set(CPP_SCHEDULER_BUILD_ENABLE_CONAN TRUE CACHE BOOL "If enabled, conan will be responsible for updating dependencies")
set(CPP_SCHEDULER_BUILD_ENABLE_COTIRE FALSE CACHE BOOL "If enabled, precompiled headers and unity builds will be enabled (provided by Cotire)")
set(CPP_SCHEDULER_BENCHMARK_DATA_DIRECTORY CACHE PATH "Path to the benchmark data directory")

set(CPP_SCHEDULER_ENGINE TRUE CACHE BOOL "If enabled, engine will be built")
set(CPP_SCHEDULER_PERSISTENCE TRUE CACHE BOOL "If enabled, loaders and persisters will be built")
set(CPP_SCHEDULER_SERVICES TRUE CACHE BOOL "If enabled, services like routing will be built")
set(CPP_SCHEDULER_TOOLS TRUE CACHE BOOL "If enabled, utilitary tools will be built")
set(CPP_SCHEDULER_TESTS FALSE CACHE BOOL "If enabled, tests will be built")

if(CPP_SCHEDULER_BENCHMARK_DATA_DIRECTORY)
	message(STATUS "Benchmarks data directory is set: ${CPP_SCHEDULER_BENCHMARK_DATA_DIRECTORY}")
	
    set(CPP_SCHEDULER_TESTS_BENCHMARKS TRUE CACHE BOOL "If enabled, benchmarks will be built")
else()
	message(STATUS "Benchmarks data directory is unset. Benchmarks are unavaiable.")
endif()

if(NOT CPP_SCHEDULER_BUILD_ENABLE_CONAN)
	set(CPP_SCHEDULER_DEPENDENCY_CATCH_ROOT_DIRECTORY CACHE PATH "Path to the catch unit test framework root directory")
	set(CPP_SCHEDULER_DEPENDENCY_COTIRE_ROOT_DIRECTORY CACHE PATH "Path to the cotire cmake module root directory")
endif()

if(CPP_SCHEDULER_BUILD_ENABLE_CONAN)
    set(CPP_SCHEDULER_FORCE_DEPENDENCIES_UPDATE FALSE CACHE BOOL "If enabled, dependencies will be forced to be updated on every cmake run. Otherwise, they will be only updated when the conan file has changed.")

    file(TIMESTAMP "${CPP_SCHEDULER_SOURCE_DIR}/conanfile.txt" LAST_CONAN_UPDATE)

    set(CONAN_UPDATE_NEEDED FALSE)

    if(CPP_SCHEDULER_FORCE_DEPENDENCIES_UPDATE)
        message(STATUS "Conan update is forced")
        set(CONAN_UPDATE_NEEDED TRUE)
    endif()
    if(NOT CPP_SCHEDULER_LAST_CONAN_FILE_UPDATE STREQUAL LAST_CONAN_UPDATE)
        message(STATUS "Conanfile has changed. Dependencies update is scheduled")
        set(CONAN_UPDATE_NEEDED TRUE)
    endif()
	if(NOT CMAKE_BUILD_TYPE STREQUAL CPP_SCHEDULER_LAST_BUILD_TYPE)
        message(STATUS "Build type has changed. Dependencies update is scheduled")
        set(CONAN_UPDATE_NEEDED TRUE)
    endif()
    
    set(CPP_SCHEDULER_LAST_CONAN_FILE_UPDATE ${LAST_CONAN_UPDATE} CACHE INTERNAL "")
    set(CPP_SCHEDULER_LAST_BUILD_TYPE ${LAST_BUILD_TYPE} CACHE INTERNAL "")
endif()

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

if(CMAKE_BUILD_TYPE STREQUAL Debug)
	add_definitions("-DDEBUG_LOGGING")
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

separate_arguments(TESTVAR WINDOWS_COMMAND "${__CONAN_ARGS}")

if(CPP_SCHEDULER_BUILD_ENABLE_CONAN)
    if(CONAN_UPDATE_NEEDED)
        message(STATUS "Updating dependencies")
        execute_process(COMMAND conan ${TESTVAR} WORKING_DIRECTORY ${CPP_SCHEDULER_SOURCE_DIR})
    endif()
    
    include(conanbuildinfo.cmake)

    conan_basic_setup()
endif()

set_property(GLOBAL PROPERTY USE_FOLDERS ON)

if(CPP_SCHEDULER_TESTS)
    enable_testing()
endif()

if(NOT CPP_SCHEDULER_BUILD_ENABLE_CONAN)
	include_directories(${CPP_SCHEDULER_DEPENDENCY_CATCH_ROOT_DIRECTORY})
endif()

include_directories(${CPP_SCHEDULER_SOURCE_DIR})
include_directories(${CPP_SCHEDULER_BINARY_DIR}/SharedLibrariesExportHeaders/)
include_directories(${CPP_SCHEDULER_SOURCE_DIR}/Tests/IncludeGroups)

set(EXECUTABLE_OUTPUT_PATH ${CPP_SCHEDULER_BINARY_DIR}/bin)
set(LIBRARY_OUTPUT_PATH ${CPP_SCHEDULER_BINARY_DIR}/lib)

if(CPP_SCHEDULER_BUILD_ENABLE_COTIRE)
	if(CPP_SCHEDULER_BUILD_ENABLE_CONAN)
		include(cmake/cotire/cotire)
	else()
		include(${DEPENDENCY_COTIRE_ROOT_DIRECTORY}/Cotire.cmake)
	endif()
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

if(CPP_SCHEDULER_TESTS_BENCHMARKS)
    add_subdirectory(Benchmarks)
endif()