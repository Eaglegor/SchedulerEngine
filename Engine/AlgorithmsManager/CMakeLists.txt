add_library(AlgorithmsManager SHARED
	AlgorithmsManager.h
	AlgorithmsManager.cpp
	
	Algorithm.h
	CostFunction.h
	
	TSPSolver.h
	VRPSolver.h
)

set_target_properties(AlgorithmsManager PROPERTIES FOLDER "Engine")

target_link_libraries(AlgorithmsManager Concepts MemoryManager)

cotire(AlgorithmsManager)

include(GenerateExportHeader)
generate_export_header(AlgorithmsManager EXPORT_FILE_NAME ${CPP_SCHEDULER_BINARY_DIR}/SharedLibrariesExportHeaders/AlgorithmsManager_export.h)
