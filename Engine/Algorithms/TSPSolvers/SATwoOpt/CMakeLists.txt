add_library(SATwoOptTSPSolver SHARED
	
	SATwoOptTSPSolver.h
	SATwoOptTSPSolver.cpp
	AcceptanceFunction.h
	BasicAcceptanceFunction.h
	BasicAcceptanceFunction.cpp
)

set_target_properties(SATwoOptTSPSolver PROPERTIES FOLDER "Engine/Algorithms/TSPSolvers/SATwoOpt")

target_link_libraries(SATwoOptTSPSolver SceneManager SceneEditor)

cotire(SATwoOptTSPSolver)

include(GenerateExportHeader)
generate_export_header(SATwoOptTSPSolver EXPORT_FILE_NAME ${CPP_SCHEDULER_BINARY_DIR}/SharedLibrariesExportHeaders/SATwoOptTSPSolver_export.h)
