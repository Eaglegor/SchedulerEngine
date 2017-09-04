add_library(HybridOptTSPSolver SHARED
	
	HybridOptTSPSolver.h
	HybridOptTSPSolver.cpp
)

set_target_properties(HybridOptTSPSolver PROPERTIES FOLDER "Engine/Algorithms/TSPSolvers/HybridOpt")

target_link_libraries(HybridOptTSPSolver SceneManager SceneEditor)

cotire(HybridOptTSPSolver)

include(GenerateExportHeader)
generate_export_header(HybridOptTSPSolver EXPORT_FILE_NAME ${CPP_SCHEDULER_BINARY_DIR}/SharedLibrariesExportHeaders/HybridOptTSPSolver_export.h)

InstallAlgorithm(HybridOptTSPSolver)
InstallHeaders(HybridOptTSPSolver.h)