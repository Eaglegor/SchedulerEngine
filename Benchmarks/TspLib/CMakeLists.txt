FIND_PACKAGE( Boost COMPONENTS program_options REQUIRED )
add_executable(TspLib_Benchmark TspLib_Benchmark.cpp)
target_link_libraries(TspLib_Benchmark Core TspLibRoutingService TspLibSceneLoader ${Boost_LIBRARIES})
set_target_properties(TspLib_Benchmark PROPERTIES FOLDER "Benchmarks/TspLib")
add_definitions(-DTSPLIB_BENCHMARK_DATA_ROOT="${CPP_SCHEDULER_BENCHMARK_DATA_DIRECTORY}/Data/TSP/TspLib" -DCATCH_CONFIG_MAIN)

InstallBenchmark(TspLib_Benchmark)
