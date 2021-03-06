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




InstallCoreComponent(SceneManager)
InstallHeaders(
	Attribute.h
	Operation.h
	WorkOperation.h
	DepotOperation.h
	Order.h
	Performer.h
	Run.h 
	Scene.h
	SceneContext.h
	SceneManager.h
	Schedule.h 
	Depot.h 
	Location.h
	ScheduleVariant.h
    Actualizable.h
    ScheduleActualizationModel.h
	RouteActualizer.h
    ArrivalTimeActualizer.h
    DurationActualizer.h 
	ScheduleValidationModel.h
	Stop.h 
	StopVisitor.h
	ConstStopVisitor.h
	DepotStop.h 
	WorkStop.h 
	Vehicle.h
	SceneObjectsFactory.h
    Constraints/ConstraintHolder.h
	Constraints/Performer/PerformerConstraints.h 
	Constraints/Operation/OperationConstraints.h
	Constraints/Order/OrderConstraints.h
	Constraints/Scene/SceneConstraints.h
	Constraints/Schedule/ScheduleConstraints.h
	Constraints/Vehicle/VehicleConstraints.h 
	Utils/SceneCloner.h 
	Utils/InvariantWorkStopsList.h 
	Utils/StreamIO.h 
	Algorithms/RunVehicleBinder.h
	Algorithms/Actualization/ArrivalTimeActualizationAlgorithm.h
    Algorithms/Actualization/DurationActualizationAlgorithm.h
    Algorithms/Actualization/RouteActualizationAlgorithm.h
    CostFunctions/SceneCostFunction.h
    CostFunctions/ScheduleCostFunction.h
    CostFunctions/RunCostFunction.h
	Listeners/StructuralChangesListener.h
	Listeners/StructuralChangesObserver.h 
	Queries/SceneQueries.h 
	Queries/OperationStopMappingQuery.h 
)