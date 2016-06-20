set(CPP_SCHEDULER_TESTS_BENCHMARKS TRUE CACHE BOOL "If enabled, benchmarks will be built")

if(CPP_SCHEDULER_TESTS_BENCHMARKS)
    add_subdirectory(TspLib)
    add_subdirectory(Taillard)
endif()

add_subdirectory(Publishers)