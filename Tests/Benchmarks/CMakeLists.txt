set(CPP_SCHEDULER_TESTS_BENCHMARKS_TSPLIB TRUE CACHE BOOL "If enabled, tsplib benchmarks will be built")
set(CPP_SCHEDULER_TESTS_BENCHMARKS_TAILLARD TRUE CACHE BOOL "If enabled, Taillard benchmarks will be built")

if(CPP_SCHEDULER_TESTS_BENCHMARKS_TSPLIB)
    add_subdirectory(TspLib)
endif()

if(CPP_SCHEDULER_TESTS_BENCHMARKS_TAILLARD)
    add_subdirectory(Taillard)
endif()

add_subdirectory(Publishers)