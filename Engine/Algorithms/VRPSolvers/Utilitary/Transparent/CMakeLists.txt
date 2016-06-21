add_library(TransparentVRPSolver SHARED
	
	TransparentVRPSolver.h
	TransparentVRPSolver.cpp
)

set_target_properties(TransparentVRPSolver PROPERTIES FOLDER "Engine/Algorithms/VRPSolvers/Utilitary")

cotire(TransparentVRPSolver)

include(GenerateExportHeader)
generate_export_header(TransparentVRPSolver EXPORT_FILE_NAME ${CPP_SCHEDULER_BINARY_DIR}/SharedLibrariesExportHeaders/TransparentVRPSolver_export.h)

if(CPP_SCHEDULER_TESTS)
	add_executable(TransparentVRPSolver_Test 
	
		Test/TransparentVRPSolver_Test.cpp
		
		)
		
	set_property(SOURCE Test/TransparentVRPSolver_Test.cpp PROPERTY COMPILE_DEFINITIONS "CATCH_CONFIG_MAIN")
		
	add_test(NAME TransparentVRPSolver_Test COMMAND $<TARGET_FILE:TransparentVRPSolver_Test>)
	target_link_libraries(TransparentVRPSolver_Test Engine CrowFlyRoutingService)
	set_target_properties(TransparentVRPSolver_Test PROPERTIES FOLDER "Engine/Algorithms/VRPSolvers/Utilitary/Tests/")
	
	cotire(TransparentVRPSolver_Test)
endif()