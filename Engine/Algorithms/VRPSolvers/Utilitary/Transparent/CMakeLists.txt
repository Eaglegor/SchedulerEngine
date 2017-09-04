add_library(TransparentVRPSolver SHARED
	
	TransparentVRPSolver.h
	TransparentVRPSolver.cpp
)

set_target_properties(TransparentVRPSolver PROPERTIES FOLDER "Engine/Algorithms/VRPSolvers/Utilitary")

cotire(TransparentVRPSolver)


GenerateExportHeader(TransparentVRPSolver)

InstallAlgorithm(TransparentVRPSolver)
InstallHeaders(TransparentVRPSolver.h)