add_library(ScrambleTSPSolver SHARED
	
	ScrambleTSPSolver.h
	ScrambleTSPSolver.cpp
)

set_target_properties(ScrambleTSPSolver PROPERTIES FOLDER "Engine/Algorithms/TSPSolvers/Utilitary")

target_link_libraries(ScrambleTSPSolver SceneManager SceneEditor)

cotire(ScrambleTSPSolver)

include(GenerateExportHeader)
generate_export_header(ScrambleTSPSolver EXPORT_FILE_NAME ${CPP_SCHEDULER_BINARY_DIR}/SharedLibrariesExportHeaders/ScrambleTSPSolver_export.h)

InstallAlgorithm(ScrambleTSPSolver)
InstallHeaders(ScrambleTSPSolver.h)