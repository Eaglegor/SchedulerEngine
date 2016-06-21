add_library(ChainVRPSolver SHARED
	
	ChainVRPSolver.h
	ChainVRPSolver.cpp
)

set_target_properties(ChainVRPSolver PROPERTIES FOLDER "Engine/Algorithms/VRPSolvers/Utilitary")

cotire(ChainVRPSolver)

include(GenerateExportHeader)
generate_export_header(ChainVRPSolver EXPORT_FILE_NAME ${CPP_SCHEDULER_BINARY_DIR}/SharedLibrariesExportHeaders/ChainVRPSolver_export.h)

if(CPP_SCHEDULER_TESTS)
	add_executable(ChainVRPSolver_Test 
	
		Test/ChainVRPSolver_Test.cpp
		
		)
		
	set_property(SOURCE Test/ChainVRPSolver_Test.cpp PROPERTY COMPILE_DEFINITIONS "CATCH_CONFIG_MAIN")
		
	add_test(NAME ChainVRPSolver_Test COMMAND $<TARGET_FILE:ChainVRPSolver_Test>)
	target_link_libraries(ChainVRPSolver_Test Engine CrowFlyRoutingService)
	set_target_properties(ChainVRPSolver_Test PROPERTIES FOLDER "Engine/Algorithms/VRPSolvers/Utilitary/Tests/")
	
	cotire(ChainVRPSolver_Test)
endif()