add_library(OneRelocateTSPSolver SHARED
	
	OneRelocateTSPSolver.h
	OneRelocateTSPSolver.cpp
)

set_target_properties(OneRelocateTSPSolver PROPERTIES FOLDER "Engine/Algorithms/TSPSolvers/OneRelocateExperimental")

target_link_libraries(OneRelocateTSPSolver SceneManager SceneEditor)

cotire(OneRelocateTSPSolver)

include(GenerateExportHeader)
generate_export_header(OneRelocateTSPSolver EXPORT_FILE_NAME ${CPP_SCHEDULER_BINARY_DIR}/SharedLibrariesExportHeaders/OneRelocateTSPSolver_export.h)
