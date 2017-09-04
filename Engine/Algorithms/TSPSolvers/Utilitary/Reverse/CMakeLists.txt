add_library(ReverseTSPSolver SHARED
	
	ReverseTSPSolver.h
	ReverseTSPSolver.cpp
)

set_target_properties(ReverseTSPSolver PROPERTIES FOLDER "Engine/Algorithms/TSPSolvers/Utilitary")

target_link_libraries(ReverseTSPSolver SceneManager SceneEditor)

cotire(ReverseTSPSolver)

include(GenerateExportHeader)
generate_export_header(ReverseTSPSolver EXPORT_FILE_NAME ${CPP_SCHEDULER_BINARY_DIR}/SharedLibrariesExportHeaders/ReverseTSPSolver_export.h)

InstallAlgorithm(ReverseTSPSolver)
InstallHeaders(ReverseTSPSolver.h)