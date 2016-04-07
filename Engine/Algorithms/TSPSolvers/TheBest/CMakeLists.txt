add_library(TheBestTSPSolver SHARED
	
	TheBestTSPSolver.h
	TheBestTSPSolver.cpp
)

set_target_properties(TheBestTSPSolver PROPERTIES FOLDER "Engine/Algorithms/TSPSolvers/TheBest")

target_link_libraries(TheBestTSPSolver SceneManager SceneEditor)

cotire(TheBestTSPSolver)

include(GenerateExportHeader)
generate_export_header(TheBestTSPSolver EXPORT_FILE_NAME ${CPP_SCHEDULER_BINARY_DIR}/SharedLibrariesExportHeaders/TheBestTSPSolver_export.h)
