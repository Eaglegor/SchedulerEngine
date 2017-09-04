add_library(TransparentVRPSolver SHARED
	
	TransparentVRPSolver.h
	TransparentVRPSolver.cpp
)

set_target_properties(TransparentVRPSolver PROPERTIES FOLDER "Engine/Algorithms/VRPSolvers/Utilitary")

cotire(TransparentVRPSolver)

include(GenerateExportHeader)
generate_export_header(TransparentVRPSolver EXPORT_FILE_NAME ${CPP_SCHEDULER_BINARY_DIR}/SharedLibrariesExportHeaders/TransparentVRPSolver_export.h)

InstallAlgorithm(TransparentVRPSolver)
InstallHeaders(TransparentVRPSolver.h)