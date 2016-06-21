add_library(PerformerAssignedVehicleBinder SHARED
	
	PerformerAssignedVehicleBinder.h
	PerformerAssignedVehicleBinder.cpp
)

set_target_properties(PerformerAssignedVehicleBinder PROPERTIES FOLDER "Engine/Algorithms/RunVehicleBinders")

target_link_libraries(PerformerAssignedVehicleBinder SceneManager)

cotire(PerformerAssignedVehicleBinder)

include(GenerateExportHeader)
generate_export_header(PerformerAssignedVehicleBinder EXPORT_FILE_NAME ${CPP_SCHEDULER_BINARY_DIR}/SharedLibrariesExportHeaders/PerformerAssignedVehicleBinder_export.h)

if(CPP_SCHEDULER_TESTS)
	add_executable(PerformerAssignedRunVehicleBinder_Test 
	
		Test/PerformerAssignedRunVehicleBinder_Test.cpp
		
		)
		
	set_property(SOURCE Test/PerformerAssignedRunVehicleBinder_Test.cpp PROPERTY COMPILE_DEFINITIONS "CATCH_CONFIG_MAIN")
		
	add_test(NAME PerformerAssignedRunVehicleBinder_Test COMMAND $<TARGET_FILE:PerformerAssignedRunVehicleBinder_Test>)
	target_link_libraries(PerformerAssignedRunVehicleBinder_Test Engine CrowFlyRoutingService)
	set_target_properties(PerformerAssignedRunVehicleBinder_Test PROPERTIES FOLDER "Engine/Algorithms/RunVehicleBinders/Tests/")
	
	cotire(PerformerAssignedRunVehicleBinder_Test)
endif()