add_library(ChainVRPSolver SHARED
	
	ChainVRPSolver.h
	ChainVRPSolver.cpp
)

set_target_properties(ChainVRPSolver PROPERTIES FOLDER "Engine/Algorithms/VRPSolvers/Utilitary")

cotire(ChainVRPSolver)


GenerateExportHeader(ChainVRPSolver)

InstallAlgorithm(ChainVRPSolver)
InstallHeaders(ChainVRPSolver.h)