add_library(IterativeTSPSolver SHARED
	
	IterativeTSPSolver.h
	IterativeTSPSolver.cpp
)

set_target_properties(IterativeTSPSolver PROPERTIES FOLDER "Engine/Algorithms/TSPSolvers/Utilitary")

target_link_libraries(IterativeTSPSolver SceneManager SceneEditor LoggingManager)

cotire(IterativeTSPSolver)

include(GenerateExportHeader)
generate_export_header(IterativeTSPSolver EXPORT_FILE_NAME ${CPP_SCHEDULER_BINARY_DIR}/SharedLibrariesExportHeaders/IterativeTSPSolver_export.h)
