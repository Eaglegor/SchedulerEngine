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
	
	Site.h
	Site.cpp
	
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


GenerateExportHeader(Concepts)

InstallCoreComponent(Concepts)
InstallHeaders(
	Capacity.h
	Coordinate.h
	Cost.h
	Distance.h
	Duration.h
	Site.h
	Route.h
	RoutingProfile.h
	Speed.h
	SpeedDurationDistanceOperations.h
	TimePoint.h
	TimeWindow.h
)