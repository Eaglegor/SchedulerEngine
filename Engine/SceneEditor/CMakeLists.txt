add_library(SceneEditor SHARED
	
	SceneEditor.h SceneEditor.cpp
    Action.h
    Checkpoint.h Checkpoint.cpp
    Patch.h Patch.cpp
    Actions/SwapWorkStops.h Actions/SwapWorkStops.cpp
    Actions/ReverseWorkStops.h Actions/ReverseWorkStops.cpp
    Actions/MoveWorkStop.h Actions/MoveWorkStop.cpp
    Actions/ScrambleWorkStops.h Actions/ScrambleWorkStops.cpp
    Actions/RotateWorkStops.h Actions/RotateWorkStops.cpp
	Actions/MoveWorkStops.h Actions/MoveWorkStops.cpp
	Actions/AllocateOrder.h Actions/AllocateOrder.cpp
	Actions/CreateRun.h Actions/CreateRun.cpp
	Actions/DestroyRun.h Actions/DestroyRun.cpp
)

set_target_properties(SceneEditor PROPERTIES FOLDER "Engine")

target_link_libraries(SceneEditor SceneManager LoggingManager)

cotire(SceneEditor)

include(GenerateExportHeader)
generate_export_header(SceneEditor EXPORT_FILE_NAME ${CPP_SCHEDULER_BINARY_DIR}/SharedLibrariesExportHeaders/SceneEditor_export.h)

if(CPP_SCHEDULER_TESTS)
	add_executable(SceneEditor_Test
		Test/SceneEditor_Test.cpp
		Test/SceneEditor_Test_MoveRunWorkStop.cpp
		Test/SceneEditor_Test_MoveRunWorkStopsSubsequence.cpp
		Test/SceneEditor_Test_ReverseRunWorkStopsSubsequence.cpp
		Test/SceneEditor_Test_Patches.cpp
	)
		
	set_property(SOURCE Test/SceneEditor_Test.cpp PROPERTY COMPILE_DEFINITIONS "CATCH_CONFIG_MAIN")
		
	add_test(NAME SceneEditor_Test COMMAND $<TARGET_FILE:SceneEditor_Test> WORKING_DIRECTORY ${EXECUTABLE_OUTPUT_PATH})
	target_link_libraries(SceneEditor_Test Core CrowFlyRoutingService JSONSceneLoader)
	set_target_properties(SceneEditor_Test PROPERTIES FOLDER "Engine/Tests/")
	
	add_custom_command(TARGET SceneEditor_Test POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_CURRENT_SOURCE_DIR}/Test/TestScene.json ${EXECUTABLE_OUTPUT_PATH}/TestData/SceneEditor_Test/TestScene.json)
	
	cotire(SceneEditor_Test)
endif()
