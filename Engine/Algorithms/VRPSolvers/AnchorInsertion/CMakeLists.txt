add_library(AnchorInsertionVRPSolver SHARED
	
	AnchorInsertionVRPSolver.h
	AnchorInsertionVRPSolver.cpp
)

set_target_properties(AnchorInsertionVRPSolver PROPERTIES FOLDER "Engine/Algorithms/VRPSolvers/AnchorInsertion")

target_link_libraries(AnchorInsertionVRPSolver AlgorithmsManager SceneManager SceneEditor)

cotire(AnchorInsertionVRPSolver)




InstallAlgorithm(AnchorInsertionVRPSolver)
InstallHeaders(AnchorInsertionVRPSolver.h)