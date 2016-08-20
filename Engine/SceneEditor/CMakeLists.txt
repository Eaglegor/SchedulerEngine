add_library(SceneEditor SHARED
	
	SceneEditor.h SceneEditor.cpp
    Action.h
    Checkpoint.h Checkpoint.cpp
    
    Actions/ActionsImpl.h Actions/ActionsImpl.cpp
    Actions/SwapRunWorkStops.h Actions/SwapRunWorkStops.cpp
    Actions/ReverseRunWorkStopsSubsequence.h Actions/ReverseRunWorkStopsSubsequence.cpp
    Actions/MoveRunWorkStop.h Actions/MoveRunWorkStop.cpp
    Actions/ScrambleWorkStopsSubsequence.h Actions/ScrambleWorkStopsSubsequence.cpp
    Actions/RotateWorkStopsSubsequence.h Actions/RotateWorkStopsSubsequence.cpp
	Actions/MoveRunWorkStopsSubsequence.h Actions/MoveRunWorkStopsSubsequence.cpp
)

set_target_properties(SceneEditor PROPERTIES FOLDER "Engine")

target_link_libraries(SceneEditor SceneManager)

cotire(SceneEditor)

include(GenerateExportHeader)
generate_export_header(SceneEditor EXPORT_FILE_NAME ${CPP_SCHEDULER_BINARY_DIR}/SharedLibrariesExportHeaders/SceneEditor_export.h)

if(CPP_SCHEDULER_TESTS)
	add_executable(SceneEditor_Test
		Test/SceneEditor_Test.cpp
		Test/SceneEditor_Test_MoveRunWorkStop.cpp
		Test/SceneEditor_Test_MoveRunWorkStopsSubsequence.cpp
		Test/SceneEditor_Test_ReverseRunWorkStopsSubsequence.cpp
	)
		
	set_property(SOURCE Test/SceneEditor_Test.cpp PROPERTY COMPILE_DEFINITIONS "CATCH_CONFIG_MAIN")
		
	add_test(NAME SceneEditor_Test COMMAND $<TARGET_FILE:SceneEditor_Test> WORKING_DIRECTORY ${EXECUTABLE_OUTPUT_PATH})
	target_link_libraries(SceneEditor_Test Engine CrowFlyRoutingService JSONSceneLoader)
	set_target_properties(SceneEditor_Test PROPERTIES FOLDER "Engine/Tests/")
	
	add_custom_command(TARGET SceneEditor_Test POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_CURRENT_SOURCE_DIR}/Test/TestScene.json ${EXECUTABLE_OUTPUT_PATH}/TestData/SceneEditor_Test/TestScene.json)
	
	cotire(SceneEditor_Test)
endif()
