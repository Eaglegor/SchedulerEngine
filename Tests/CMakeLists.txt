add_subdirectory(Engine)
add_subdirectory(Persistence)

add_subdirectory(Utils)
add_subdirectory(IncludeGroups)

if(CPP_SCHEDULER_BENCHMARK_DATA_DIRECTORY)
	message(STATUS "Benchmarks data directory is set: ${CPP_SCHEDULER_BENCHMARK_DATA_DIRECTORY}")
	
    add_subdirectory(Benchmarks)
else()
	message(STATUS "Benchmarks data directory is unset. Benchmarks are unavaiable.")
endif()