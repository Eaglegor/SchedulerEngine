add_library(SimpleTwoOptTSPSolver SHARED
	
	SimpleTwoOptTSPSolver.h
	SimpleTwoOptTSPSolver.cpp
)

set_target_properties(SimpleTwoOptTSPSolver PROPERTIES FOLDER "Engine/Algorithms/TSPSolvers/SimpleTwoOpt")

target_link_libraries(SimpleTwoOptTSPSolver SceneManager SceneEditor)

cotire(SimpleTwoOptTSPSolver)

include(GenerateExportHeader)
generate_export_header(SimpleTwoOptTSPSolver EXPORT_FILE_NAME ${CPP_SCHEDULER_BINARY_DIR}/SharedLibrariesExportHeaders/SimpleTwoOptTSPSolver_export.h)
