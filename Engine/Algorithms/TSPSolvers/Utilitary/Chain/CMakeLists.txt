add_library(ChainTSPSolver SHARED
	
	ChainTSPSolver.h
	ChainTSPSolver.cpp
)

set_target_properties(ChainTSPSolver PROPERTIES FOLDER "Engine/Algorithms/TSPSolvers/Utilitary")

target_link_libraries(ChainTSPSolver SceneManager SceneEditor)

cotire(ChainTSPSolver)

include(GenerateExportHeader)
generate_export_header(ChainTSPSolver EXPORT_FILE_NAME ${CPP_SCHEDULER_BINARY_DIR}/SharedLibrariesExportHeaders/ChainTSPSolver_export.h)

InstallAlgorithm(ChainTSPSolver)
InstallHeaders(ChainTSPSolver.h)