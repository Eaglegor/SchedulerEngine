cmake_minimum_required(VERSION 3.2)
project(CPP_SCHEDULER)

find_package(OpenMP)
if (OPENMP_FOUND)
    set (CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${OpenMP_C_FLAGS}")
    set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${OpenMP_CXX_FLAGS}")
endif()

if(NOT MSVC)
	message(STATUS "Setting GCC/Clang compiler flags: -std=c++11 -Wno-attributes")
	set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -Wno-attributes")
else()
	add_definitions( "/wd4251 /wd4275" )
endif()

set(CPP_SCHEDULER_BUILD_AUTO_UPDATE_DEPENDENCIES TRUE CACHE BOOL "If enabled, dependencies will be automatically updated using the cmake script")
set(THIRD_PARTY_DIRECTORY ${CPP_SCHEDULER_SOURCE_DIR}/ThirdParty CACHE STRING "Path to the folder where third-party libraries will be installed")
set(THIRD_PARTY_PACKAGES_DIRECTORY ${CPP_SCHEDULER_SOURCE_DIR}/ThirdPartyPackages CACHE STRING "Path to the folder where third-party packages will be downloaded")

set(CPP_SCHEDULER_BUILD_ENABLE_COTIRE FALSE CACHE BOOL "If enabled, precompiled headers and unity builds will be enabled (provided by Cotire)")

set(CPP_SCHEDULER_BUILD_ENABLE_IWYU FALSE CACHE BOOL "If enabled, google include-what-you-use (if present) will be used during the build to suggest includes optimizations")

set(CPP_SCHEDULER_BENCHMARK_DATA_DIRECTORY CACHE PATH "Path to the benchmark data directory")

set(CPP_SCHEDULER_ENGINE TRUE CACHE BOOL "If enabled, engine will be built")
set(CPP_SCHEDULER_PERSISTENCE TRUE CACHE BOOL "If enabled, loaders and persisters will be built")
set(CPP_SCHEDULER_SERVICES TRUE CACHE BOOL "If enabled, services like routing will be built")
set(CPP_SCHEDULER_TOOLS TRUE CACHE BOOL "If enabled, utilitary tools will be built")
set(CPP_SCHEDULER_TESTS FALSE CACHE BOOL "If enabled, tests will be built")

if(CPP_SCHEDULER_BUILD_AUTO_UPDATE_DEPENDENCIES)
	include(PrepareDeps.cmake)
	UpdateDependencies()
endif()

include(InstallUtils.cmake)


include_directories(${CATCH_ROOT})
include_directories(${SPDLOG_ROOT}/include)
include_directories(${BOOST_INCLUDEDIR})
include_directories(${LIBXML2_ROOT}/include)
include_directories(${CIMG_ROOT})

if(CPP_SCHEDULER_BENCHMARK_DATA_DIRECTORY)
	message(STATUS "Benchmarks data directory is set: ${CPP_SCHEDULER_BENCHMARK_DATA_DIRECTORY}")
	
	set(CPP_SCHEDULER_TESTS_BENCHMARKS TRUE CACHE BOOL "If enabled, benchmarks will be built")
else()
	message(STATUS "Benchmarks data directory is unset. Benchmarks are unavaiable.")
endif()

cmake_policy(SET CMP0054 OLD)
cmake_policy(SET CMP0042 OLD)

include_directories(${CPP_SCHEDULER_SOURCE_DIR})
include_directories(${CPP_SCHEDULER_BINARY_DIR}/SharedLibrariesExportHeaders)
include_directories(${CPP_SCHEDULER_SOURCE_DIR}/Tests/IncludeGroups)

if(NOT CMAKE_BUILD_TYPE)
	set(CMAKE_BUILD_TYPE Debug)
endif()

if(CMAKE_BUILD_TYPE STREQUAL Debug)
	add_definitions("-DDEBUG_LOGGING")
endif()

set_property(GLOBAL PROPERTY USE_FOLDERS ON)

if(CPP_SCHEDULER_TESTS)
    enable_testing()
endif()

if(CPP_SCHEDULER_BUILD_ENABLE_COTIRE)
	include(${COTIRE_ROOT}/cotire.cmake)
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

if(UNIX)
	file(GLOB SH_LIBS "${CMAKE_INSTALL_PREFIX}/${CMAKE_BUILD_TYPE}/lib/*.so")
elseif(WIN32)
	file(GLOB SH_LIBS "${CMAKE_INSTALL_PREFIX}/${CMAKE_BUILD_TYPE}/lib/*.dll")
endif()
