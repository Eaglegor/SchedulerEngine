add_library(GreedyTSPSolver SHARED
	
	GreedyTSPSolver.h
	GreedyTSPSolver.cpp
)

set_target_properties(GreedyTSPSolver PROPERTIES FOLDER "Engine/Algorithms/TSPSolvers/Greedy")

target_link_libraries(GreedyTSPSolver SceneManager SceneEditor LoggingManager)

cotire(GreedyTSPSolver)

include(GenerateExportHeader)
generate_export_header(GreedyTSPSolver EXPORT_FILE_NAME ${CPP_SCHEDULER_BINARY_DIR}/SharedLibrariesExportHeaders/GreedyTSPSolver_export.h)
