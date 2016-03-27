add_executable(BTest_TspLib BTest_TspLib.cpp)

set(CPP_SCHEDULER_TESTS_BENCHMARKS_TSPLIB_HEAVY FALSE CACHE BOOL "If enabled, heavy (500-1000 cities) and huge (>1000 cities) datasets will be enabled for tsplib")

add_test(NAME BTest_TspLib COMMAND $<TARGET_FILE:BTest_TspLib>)

if(CPP_SCHEDULER_TESTS_BENCHMARKS_TSPLIB_HEAVY)
    add_test(NAME BTest_TspLib_Heavy COMMAND $<TARGET_FILE:BTest_TspLib> tsplib_heavy tsplib_huge)
endif()

target_link_libraries(BTest_TspLib Engine TspLibRoutingService TspLibSceneLoader)
set_target_properties(BTest_TspLib PROPERTIES FOLDER "Tests/Benchmark/TspLib")
add_definitions(-DTSPLIB_BENCHMARK_DATA_ROOT="${CPP_SCHEDULER_BENCHMARK_DATA_DIRECTORY}/Data/TspLib")
