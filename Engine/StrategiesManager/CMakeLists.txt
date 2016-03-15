add_library(StrategiesManager SHARED
	StrategiesManager.h
	StrategiesManager.cpp
	
	Strategy.h
	Strategy.cpp
	
	TSPSolver.h
	VRPSolver.h
    
    CostFunctions/SceneCostFunction.h
    CostFunctions/ScheduleCostFunction.h
    CostFunctions/RunCostFunction.h
)

set_target_properties(StrategiesManager PROPERTIES FOLDER "Engine")

target_link_libraries(StrategiesManager Concepts MemoryManager)

include(GenerateExportHeader)
generate_export_header(StrategiesManager EXPORT_FILE_NAME ${CPP_SCHEDULER_BINARY_DIR}/SharedLibrariesExportHeaders/StrategiesManager_export.h)
