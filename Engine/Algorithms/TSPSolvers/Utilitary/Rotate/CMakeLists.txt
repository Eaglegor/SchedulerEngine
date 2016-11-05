add_library(RotateTSPSolver SHARED
	
	RotateTSPSolver.h
	RotateTSPSolver.cpp
)

set_target_properties(RotateTSPSolver PROPERTIES FOLDER "Engine/Algorithms/TSPSolvers/Utilitary")

target_link_libraries(RotateTSPSolver SceneManager SceneEditor)

cotire(RotateTSPSolver)

include(GenerateExportHeader)
generate_export_header(RotateTSPSolver EXPORT_FILE_NAME ${CPP_SCHEDULER_BINARY_DIR}/SharedLibrariesExportHeaders/RotateTSPSolver_export.h)
