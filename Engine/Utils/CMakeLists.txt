add_custom_target(Utils SOURCES
	Units/DurationUnits.h
	Units/SpeedUnits.h
	
	Collections/Algorithms.h
	Collections/ImmutableUnorderedSet.h
	Collections/ImmutableVector.h
	
    Aggregators/DistanceAccumulator.h
    Aggregators/DurationAccumulator.h
    
	Factory.h
    CallableVisitorProxy.h
)

set_target_properties(Utils PROPERTIES FOLDER "Engine")

if(CPP_SCHEDULER_TESTS)
	add_executable(Utils_Test 
	
		Test/Collections_Algorithms_Test.cpp
		Test/DurationUnits_Test.cpp
		Test/SpeedUnits_Test.cpp
		
		)
		
	set_property(SOURCE Test/Collections_Algorithms_Test.cpp PROPERTY COMPILE_DEFINITIONS "CATCH_CONFIG_MAIN")
		
	add_test(NAME Utils_Test COMMAND $<TARGET_FILE:Utils_Test>)
	target_link_libraries(Utils_Test Engine CrowFlyRoutingService)
	set_target_properties(Utils_Test PROPERTIES FOLDER "Engine/Tests/")
	
	cotire(Utils_Test)
endif()