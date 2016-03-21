set(CPP_SCHEDULER_TESTS_BENCHMARKS_TSPLIB TRUE CACHE BOOL "If enabled, tsplib benchmarks will be built")

if(CPP_SCHEDULER_TESTS_BENCHMARKS_TSPLIB)
    add_subdirectory(TspLib)
endif()