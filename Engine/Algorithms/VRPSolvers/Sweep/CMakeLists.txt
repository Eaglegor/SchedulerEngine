add_library(SweepVRPSolver SHARED
	
	SweepVRPSolver.h
	SweepVRPSolver.cpp
)

set_target_properties(SweepVRPSolver PROPERTIES FOLDER "Engine/Algorithms/VRPSolvers/Sweep")

target_link_libraries(SweepVRPSolver StrategiesManager SceneManager)

cotire(SweepVRPSolver)

include(GenerateExportHeader)
generate_export_header(SweepVRPSolver EXPORT_FILE_NAME ${CPP_SCHEDULER_BINARY_DIR}/SharedLibrariesExportHeaders/SweepVRPSolver.h)
