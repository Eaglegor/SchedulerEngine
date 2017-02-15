add_library(SimpleEAVRPSolver SHARED
	
	SimpleEAVRPSolver.h SimpleEAVRPSolver.cpp
	RandomSolutionGenerator.h RandomSolutionGenerator.cpp
	Evolution.h Evolution.cpp

	)

set_target_properties(SimpleEAVRPSolver PROPERTIES FOLDER "Engine/Algorithms/VRPSolvers/Evolutionary/SingleObjective")

target_link_libraries(SimpleEAVRPSolver AlgorithmsManager SceneManager SceneEditor)

cotire(SimpleEAVRPSolver)

include(GenerateExportHeader)
generate_export_header(SimpleEAVRPSolver EXPORT_FILE_NAME ${CPP_SCHEDULER_BINARY_DIR}/SharedLibrariesExportHeaders/SimpleEAVRPSolver_export.h)

