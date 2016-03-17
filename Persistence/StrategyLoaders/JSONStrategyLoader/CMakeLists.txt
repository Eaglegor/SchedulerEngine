add_library(JSONStrategyLoader SHARED
		JSONStrategyLoader.h
		JSONStrategyLoader.cpp
		
		Loaders/VRPSolvers/VRPSolverLoader.h
		
		Loaders/TSPSolvers/TSPSolverLoader.h
)

target_link_libraries(JSONStrategyLoader Engine)

set_target_properties(JSONStrategyLoader PROPERTIES FOLDER "Persistence/SceneLoaders")

include(GenerateExportHeader)
generate_export_header(JSONStrategyLoader EXPORT_FILE_NAME ${CPP_SCHEDULER_BINARY_DIR}/SharedLibrariesExportHeaders/JSONStrategyLoader_export.h)
