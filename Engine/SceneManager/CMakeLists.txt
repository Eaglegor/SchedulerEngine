add_library(SceneManager SHARED
	
	Attribute.h
	Attribute.cpp
	
	Operation.h
	Operation.cpp
	
	Order.h
	Order.cpp
	
	Performer.h
	Performer.cpp
	
	Run.h
	Run.cpp
	
	Scene.h
	Scene.cpp
	
	SceneManager.h
	SceneManager.cpp
	
	Schedule.h
	Schedule.cpp
	
	ScheduleStateUtils.h
	ScheduleStateUtils.cpp

	TemporarySchedule.h TemporarySchedule.cpp
	
    Actualizable.h
    ScheduleActualizationModel.h ScheduleActualizationModel.cpp
	RouteActualizer.h
    ArrivalTimeActualizer.h
    DurationActualizer.h
    
	ScheduleValidationModel.h ScheduleValidationModel.cpp
	
	Stop.h
	Stop.cpp
	
	StopVisitor.h
    ConstStopVisitor.h
	
	RunBoundaryStop.h RunBoundaryStop.cpp
	WorkStop.h WorkStop.cpp
	
	Vehicle.h
	Vehicle.cpp
	
	SceneObjectsFactory.h

	Extensions/TimeWindowSelector.h
	Extensions/RunVehicleBinder.h
	Extensions/ArrivalTimeActualizationAlgorithm.h
    Extensions/DurationActualizationAlgorithm.h
    Extensions/RouteActualizationAlgorithm.h
	Extensions/StopValidationAlgorithm.h
	Extensions/RunValidationAlgorithm.h
	Extensions/ScheduleValidationAlgorithm.h
    
)

set_target_properties(SceneManager PROPERTIES FOLDER "Engine")

target_link_libraries(SceneManager Concepts MemoryManager)

cotire(SceneManager)

include(GenerateExportHeader)
generate_export_header(SceneManager EXPORT_FILE_NAME ${CPP_SCHEDULER_BINARY_DIR}/SharedLibrariesExportHeaders/SceneManager_export.h)


if(CPP_SCHEDULER_TESTS)
	add_executable(SceneManager_Test 
	
		Test/SceneManager_Test.cpp

	)
		
	set_property(SOURCE Test/SceneManager_Test.cpp PROPERTY COMPILE_DEFINITIONS "CATCH_CONFIG_MAIN")		
		
	add_test(NAME SceneManager_Test COMMAND $<TARGET_FILE:SceneManager_Test>)
	target_link_libraries(SceneManager_Test SceneManager CrowFlyRoutingService)
	set_target_properties(SceneManager_Test PROPERTIES FOLDER "Engine/Tests/")
	
	cotire(SceneManager_Test)
endif()