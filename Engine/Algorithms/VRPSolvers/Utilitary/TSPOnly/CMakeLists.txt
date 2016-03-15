add_library(TSPOnlyVRPSolver SHARED
	
	TSPOnlyVRPSolver.h
	TSPOnlyVRPSolver.cpp
)

set_target_properties(TSPOnlyVRPSolver PROPERTIES FOLDER "Engine/Algorithms/VRPSolvers/Utilitary")

target_link_libraries(TSPOnlyVRPSolver StrategiesManager SceneManager)

include(GenerateExportHeader)
generate_export_header(TSPOnlyVRPSolver EXPORT_FILE_NAME ${CPP_SCHEDULER_BINARY_DIR}/SharedLibrariesExportHeaders/TSPOnlyVRPSolver.h)
