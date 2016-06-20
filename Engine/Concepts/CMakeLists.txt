add_library(Concepts SHARED

	Capacity.h
	Capacity.cpp
	
	Coordinate.h
	Coordinate.cpp
	
	Cost.h
	Cost.cpp
	
	Distance.h
	Distance.cpp
	
	Duration.h
	Duration.cpp
	
	Location.h
	Location.cpp
	
	Route.h
	Route.cpp
	
	RoutingProfile.h
	RoutingProfile.cpp
	
	Speed.h
	Speed.cpp
	
	SpeedDurationDistanceOperations.h
	SpeedDurationDistanceOperations.cpp
	
	TimePoint.h
	TimePoint.cpp
	
	TimeWindow.h
	TimeWindow.cpp
)

set_target_properties(Concepts PROPERTIES FOLDER "Engine")

cotire(Concepts)

include(GenerateExportHeader)
generate_export_header(Concepts EXPORT_FILE_NAME ${CPP_SCHEDULER_BINARY_DIR}/SharedLibrariesExportHeaders/Concepts_export.h)


if(CPP_SCHEDULER_TESTS)
	add_executable(Concepts_Test 
	
		Test/Concepts_Test_Capacity.cpp
		Test/Concepts_Test_Coordinate.cpp
		Test/Concepts_Test_Cost.cpp
		Test/Concepts_Test_Distance.cpp
		Test/Concepts_Test_Duration.cpp
		Test/Concepts_Test_Location.cpp
		Test/Concepts_Test_Route.cpp
		Test/Concepts_Test_TimePoint.cpp
		Test/Concepts_Test_RoutingProfile.cpp
		Test/Concepts_Test_Speed.cpp
		Test/Concepts_Test_TimeWindow.cpp
		Test/Concepts_Test_SpeedDurationDistanceOperations.cpp
		
		)
		
	set_property(SOURCE Test/Concepts_Test_Capacity.cpp PROPERTY COMPILE_DEFINITIONS "CATCH_CONFIG_MAIN")
		
	add_test(NAME Concepts_Test COMMAND $<TARGET_FILE:Concepts_Test>)
	target_link_libraries(Concepts_Test Concepts)
	set_target_properties(Concepts_Test PROPERTIES FOLDER "Engine/Tests/")
endif()
