add_library(TransparentTSPSolver SHARED
	
	TransparentTSPSolver.h
	TransparentTSPSolver.cpp
)

set_target_properties(TransparentTSPSolver PROPERTIES FOLDER "Engine/Algorithms/TSPSolvers/Utilitary")

target_link_libraries(TransparentTSPSolver SceneManager)

cotire(TransparentTSPSolver)

include(GenerateExportHeader)
generate_export_header(TransparentTSPSolver EXPORT_FILE_NAME ${CPP_SCHEDULER_BINARY_DIR}/SharedLibrariesExportHeaders/TransparentTSPSolver_export.h)

InstallAlgorithm(TransparentTSPSolver)
InstallHeaders(TransparentTSPSolver.h)