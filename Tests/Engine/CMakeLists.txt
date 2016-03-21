set(CPP_SCHEDULER_TESTS_ENGINE_CONCEPTS TRUE CACHE BOOL "If enabled, concepts tests will be built")
set(CPP_SCHEDULER_TESTS_ENGINE_SCENEMANAGER TRUE CACHE BOOL "If enabled, scene manager tests will be built")
set(CPP_SCHEDULER_TESTS_ENGINE_STRATEGIESMANAGER TRUE CACHE BOOL "If enabled, strategies manager tests will be built")
set(CPP_SCHEDULER_TESTS_ENGINE_UTILS TRUE CACHE BOOL "If enabled, utils tests will be built")
set(CPP_SCHEDULER_TESTS_ENGINE_SCENEEDITOR TRUE CACHE BOOL "If enabled, scene editor tests will be built")

if(CPP_SCHEDULER_TESTS_ENGINE_CONCEPTS)
    add_subdirectory(Concepts)
endif()

if(CPP_SCHEDULER_TESTS_ENGINE_SCENEMANAGER)
    add_subdirectory(SceneManager)
endif()

if(CPP_SCHEDULER_TESTS_ENGINE_STRATEGIESMANAGER)
    add_subdirectory(StrategiesManager)
endif()

if(CPP_SCHEDULER_TESTS_ENGINE_UTILS)
    add_subdirectory(Utils)
endif()

if(CPP_SCHEDULER_TESTS_ENGINE_SCENEEDITOR)
    add_subdirectory(SceneEditor)
endif()