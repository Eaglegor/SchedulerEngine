add_subdirectory(Engine)
add_subdirectory(Persistence)
add_subdirectory(Utils)

if(CPP_SCHEDULER_BENCHMARK_DATA_DIRECTORY)
	message(STATUS "Benchmarks data directory is set: ${CPP_SCHEDULER_BENCHMARK_DATA_DIRECTORY}")
	message(STATUS "Loading benchmark tests")
	
	add_subdirectory(Benchmarks)
else()
	message(STATUS "Benchmarks data directory is unset. Skipping benchmark tests.")
endif()