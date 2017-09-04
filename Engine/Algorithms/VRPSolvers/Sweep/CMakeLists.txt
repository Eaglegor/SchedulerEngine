add_library(SweepVRPSolver SHARED
	
	SweepVRPSolver.h
	SweepVRPSolver.cpp
)

set_target_properties(SweepVRPSolver PROPERTIES FOLDER "Engine/Algorithms/VRPSolvers/Sweep")

target_link_libraries(SweepVRPSolver AlgorithmsManager SceneManager SceneEditor)

cotire(SweepVRPSolver)


GenerateExportHeader(SweepVRPSolver)

InstallAlgorithm(SweepVRPSolver)
InstallHeaders(SweepVRPSolver.h)