add_library(Core SHARED
	Engine.h
	Engine.cpp
)

target_link_libraries(Core
    SceneManager 
	SceneEditor
    AlgorithmsManager
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
    
    TransparentVRPSolver
    ChainVRPSolver
    TSPOnlyVRPSolver
	SweepVRPSolver
	CWSavingsVRPSolver
	AnchorInsertionVRPSolver
    
	SimpleTwoOptTSPSolver
	SimulatedAnnealingTSPSolver
	GreedyTSPSolver
	ChainTSPSolver
	OneRelocateTSPSolver
	BestOfTSPSolver
	HybridOptTSPSolver
	SuIntTSPSolver
	TransparentTSPSolver
	ReverseTSPSolver
	IterativeTSPSolver
	ScrambleTSPSolver
	RotateTSPSolver
	
	TotalDistanceCostFunctions
)

set_target_properties(Core PROPERTIES FOLDER "Core")

cotire(Core)




InstallCoreComponent(Core)
InstallHeaders(Engine.h)