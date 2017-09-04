add_library(ChainVRPSolver SHARED
	
	ChainVRPSolver.h
	ChainVRPSolver.cpp
)

set_target_properties(ChainVRPSolver PROPERTIES FOLDER "Engine/Algorithms/VRPSolvers/Utilitary")

cotire(ChainVRPSolver)




InstallAlgorithm(ChainVRPSolver)
InstallHeaders(ChainVRPSolver.h)