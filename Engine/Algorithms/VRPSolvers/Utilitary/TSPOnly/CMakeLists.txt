add_library(TSPOnlyVRPSolver SHARED
	
	TSPOnlyVRPSolver.h
	TSPOnlyVRPSolver.cpp
)

set_target_properties(TSPOnlyVRPSolver PROPERTIES FOLDER "Engine/Algorithms/VRPSolvers/Utilitary")

target_link_libraries(TSPOnlyVRPSolver AlgorithmsManager SceneManager)

cotire(TSPOnlyVRPSolver)




InstallAlgorithm(TSPOnlyVRPSolver)
InstallHeaders(TSPOnlyVRPSolver.h)