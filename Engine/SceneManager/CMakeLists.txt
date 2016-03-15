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
	
	ScheduleActualizer.h
	ScheduleActualizer.cpp
	
	Stop.h
	Stop.cpp
	
	Vehicle.h
	Vehicle.cpp
	
	SceneObjectsFactory.h
	
	Views/RunStopsView.h
	Views/RunStopsView.cpp
	
	Views/ScheduleStopsView.h
	Views/ScheduleStopsView.cpp
	
	Extensions/TimeWindowSelector.h
	Extensions/RunVehicleBinder.h
	Extensions/ScheduleActualizationAlgorithm.h
)

set_target_properties(SceneManager PROPERTIES FOLDER "Engine")

target_link_libraries(SceneManager Concepts MemoryManager)

include(GenerateExportHeader)
generate_export_header(SceneManager EXPORT_FILE_NAME ${CPP_SCHEDULER_BINARY_DIR}/SharedLibrariesExportHeaders/SceneManager_export.h)
