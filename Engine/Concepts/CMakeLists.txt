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

include(GenerateExportHeader)
generate_export_header(Concepts EXPORT_FILE_NAME ${CPP_SCHEDULER_BINARY_DIR}/SharedLibrariesExportHeaders/Concepts_export.h)
