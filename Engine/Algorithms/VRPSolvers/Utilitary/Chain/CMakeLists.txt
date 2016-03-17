add_library(ChainVRPSolver SHARED
	
	ChainVRPSolver.h
	ChainVRPSolver.cpp
)

set_target_properties(ChainVRPSolver PROPERTIES FOLDER "Engine/Algorithms/VRPSolvers/Utilitary")

include(GenerateExportHeader)
generate_export_header(ChainVRPSolver EXPORT_FILE_NAME ${CPP_SCHEDULER_BINARY_DIR}/SharedLibrariesExportHeaders/ChainVRPSolver_export.h)
