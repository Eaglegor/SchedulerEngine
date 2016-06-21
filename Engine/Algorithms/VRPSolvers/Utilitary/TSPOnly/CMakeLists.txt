add_library(TSPOnlyVRPSolver SHARED
	
	TSPOnlyVRPSolver.h
	TSPOnlyVRPSolver.cpp
)

set_target_properties(TSPOnlyVRPSolver PROPERTIES FOLDER "Engine/Algorithms/VRPSolvers/Utilitary")

target_link_libraries(TSPOnlyVRPSolver StrategiesManager SceneManager)

cotire(TSPOnlyVRPSolver)

include(GenerateExportHeader)
generate_export_header(TSPOnlyVRPSolver EXPORT_FILE_NAME ${CPP_SCHEDULER_BINARY_DIR}/SharedLibrariesExportHeaders/TSPOnlyVRPSolver.h)

if(CPP_SCHEDULER_TESTS)
	add_executable(TSPOnlyVRPSolver_Test 
	
		Test/TSPOnlyVRPSolver_Test.cpp
		
		)
		
	set_property(SOURCE Test/TSPOnlyVRPSolver_Test.cpp PROPERTY COMPILE_DEFINITIONS "CATCH_CONFIG_MAIN")
		
	add_test(NAME TSPOnlyVRPSolver_Test COMMAND $<TARGET_FILE:TSPOnlyVRPSolver_Test>)
	target_link_libraries(TSPOnlyVRPSolver_Test Engine CrowFlyRoutingService)
	set_target_properties(TSPOnlyVRPSolver_Test PROPERTIES FOLDER "Engine/Algorithms/VRPSolvers/Utilitary/Tests/")
	
	cotire(TSPOnlyVRPSolver_Test)
endif()