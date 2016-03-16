add_library(Engine SHARED
	
	Engine.h
	Engine.cpp
	
	EngineContext.h
	
	Services/RoutingService.h
)

target_link_libraries(Engine 
    SceneManager 
    StrategiesManager
    MemoryManager
    
    PerformerAssignedVehicleBinder
    
    StopDurationActualizationAlgorithm
    StopArrivalTimeActualizationAlgorithm
    
    FirstTimeWindowSelector
    
    TransparentVRPSolver
    ChainVRPSolver
    TSPOnlyVRPSolver
    
    )

set_target_properties(Engine PROPERTIES FOLDER "Engine")

include(GenerateExportHeader)
generate_export_header(Engine EXPORT_FILE_NAME ${CPP_SCHEDULER_BINARY_DIR}/SharedLibrariesExportHeaders/Engine_export.h)