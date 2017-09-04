add_library(BestOfTSPSolver SHARED
	
	BestOfTSPSolver.h
	BestOfTSPSolver.cpp
)

set_target_properties(BestOfTSPSolver PROPERTIES FOLDER "Engine/Algorithms/TSPSolvers/Utilitary")

target_link_libraries(BestOfTSPSolver SceneManager SceneEditor LoggingManager)

cotire(BestOfTSPSolver)

include(GenerateExportHeader)
generate_export_header(BestOfTSPSolver EXPORT_FILE_NAME ${CPP_SCHEDULER_BINARY_DIR}/SharedLibrariesExportHeaders/BestOfTSPSolver_export.h)

InstallAlgorithm(BestOfTSPSolver)
InstallHeaders(BestOfTSPSolver.h)