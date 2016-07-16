add_library(Engine SHARED
	
	Engine.h
	Engine.cpp
	
	Services/RoutingService.h
)

target_link_libraries(Engine 
    SceneManager 
	SceneEditor
    StrategiesManager
    MemoryManager
    
    PerformerAssignedVehicleBinder
    
	DefaultRouteActualizationAlgorithm
    DefaultDurationActualizationAlgorithm
    DefaultArrivalTimeActualizationAlgorithm
	
	ChainScheduleValidationAlgorithm
	ValidRunsScheduleValidationAlgorithm
	
	ChainRunValidationAlgorithm
	OverloadRunValidationAlgorithm
	ValidStopsRunValidationAlgorithm
	
	ChainStopValidationAlgorithm
    
    FirstTimeWindowSelector
    
    TransparentVRPSolver
    ChainVRPSolver
    TSPOnlyVRPSolver
	SweepVRPSolver
    
	SimpleTwoOptTSPSolver
	SimulatedAnnealingTSPSolver
	GreedyTSPSolver
	ChainTSPSolver
	OneRelocateTSPSolver
	TheBestTSPSolver
	HybridOptTSPSolver
	SuIntTSPSolver
	
	TotalDistanceCostFunctions
    )

set_target_properties(Engine PROPERTIES FOLDER "Engine")

cotire(Engine)

include(GenerateExportHeader)
generate_export_header(Engine EXPORT_FILE_NAME ${CPP_SCHEDULER_BINARY_DIR}/SharedLibrariesExportHeaders/Engine_export.h)
