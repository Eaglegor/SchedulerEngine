add_library(PerformerAssignedVehicleBinder SHARED
	
	PerformerAssignedVehicleBinder.h
	PerformerAssignedVehicleBinder.cpp
)

set_target_properties(PerformerAssignedVehicleBinder PROPERTIES FOLDER "Engine/Algorithms/RunVehicleBinders")

target_link_libraries(PerformerAssignedVehicleBinder SceneManager)

cotire(PerformerAssignedVehicleBinder)


InstallAlgorithm(PerformerAssignedVehicleBinder)
InstallHeaders(PerformerAssignedVehicleBinder.h)
