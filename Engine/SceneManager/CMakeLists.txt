add_library(SceneManager SHARED
	
	Attribute.h Attribute.cpp
	
	Operation.h Operation.cpp
	
	WorkOperation.h WorkOperation.cpp
	
	DepotOperation.h DepotOperation.cpp
	
	Order.h Order.cpp
	
	Performer.h Performer.cpp
	
	Run.h Run.cpp
	
	Scene.h Scene.cpp
	SceneContext.h SceneContext.cpp
	
	SceneManager.h SceneManager.cpp
	
	Schedule.h Schedule.cpp
	
	Depot.h Depot.cpp
	Location.h Location.cpp

	ScheduleVariant.h ScheduleVariant.cpp
	
    Actualizable.h
    ScheduleActualizationModel.h ScheduleActualizationModel.cpp
	RouteActualizer.h RouteActualizer.cpp
    ArrivalTimeActualizer.h ArrivalTimeActualizer.cpp
    DurationActualizer.h DurationActualizer.cpp
    
	ScheduleValidationModel.h ScheduleValidationModel.cpp
	
	Stop.h Stop.cpp
	
	StopVisitor.h  ConstStopVisitor.h
	
	DepotStop.h DepotStop.cpp
	WorkStop.h WorkStop.cpp
	
	Vehicle.h Vehicle.cpp
	
	SceneObjectsFactory.h

    Constraints/ConstraintHolder.h
    
	Constraints/Performer/PerformerConstraints.h Constraints/Performer/PerformerConstraints.cpp
	
	Constraints/Operation/OperationConstraints.h Constraints/Operation/OperationConstraints.cpp
	
	Constraints/Order/OrderConstraints.h Constraints/Order/OrderConstraints.cpp
	
	Constraints/Scene/SceneConstraints.h Constraints/Scene/SceneConstraints.cpp
	
	Constraints/Schedule/ScheduleConstraints.h Constraints/Schedule/ScheduleConstraints.cpp
	
	Constraints/Vehicle/VehicleConstraints.h Constraints/Vehicle/VehicleConstraints.cpp
	
	Utils/SceneCloner.h Utils/SceneCloner.cpp
	Utils/InvariantWorkStopsList.h Utils/InvariantWorkStopsList.cpp
	Utils/StreamIO.h Utils/StreamIO.cpp
	
	Algorithms/RunVehicleBinder.h
	Algorithms/Actualization/ArrivalTimeActualizationAlgorithm.h
    Algorithms/Actualization/DurationActualizationAlgorithm.h
    Algorithms/Actualization/RouteActualizationAlgorithm.h
	
    CostFunctions/SceneCostFunction.h
    CostFunctions/ScheduleCostFunction.h
    CostFunctions/RunCostFunction.h

	Listeners/StructuralChangesListener.h
	Listeners/StructuralChangesObserver.h Listeners/StructuralChangesObserver.cpp
	
	Queries/SceneQueries.h Queries/SceneQueries.cpp
	Queries/OperationStopMappingQuery.h Queries/OperationStopMappingQuery.cpp
)

set_target_properties(SceneManager PROPERTIES FOLDER "Engine")

target_link_libraries(SceneManager Concepts MemoryManager)

cotire(SceneManager)

include(GenerateExportHeader)
generate_export_header(SceneManager EXPORT_FILE_NAME ${CPP_SCHEDULER_BINARY_DIR}/SharedLibrariesExportHeaders/SceneManager_export.h)

