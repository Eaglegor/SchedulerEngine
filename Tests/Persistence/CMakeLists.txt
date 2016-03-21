set(CPP_SCHEDULER_TESTS_PERSISTENCE_SCENELOADERS TRUE CACHE BOOL "If enabled, scene loaders tests will be built")
set(CPP_SCHEDULER_TESTS_PERSISTENCE_STRATEGYLOADERS TRUE CACHE BOOL "If enabled, strategy loaders tests will be built")

if(CPP_SCHEDULER_TESTS_PERSISTENCE_SCENELOADERS)
    add_subdirectory(SceneLoaders)
endif()

if(CPP_SCHEDULER_TESTS_PERSISTENCE_STRATEGYLOADERS)
    add_subdirectory(StrategyLoaders)
endif()