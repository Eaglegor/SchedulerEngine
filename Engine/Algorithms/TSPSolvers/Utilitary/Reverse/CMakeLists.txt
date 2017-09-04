add_library(ReverseTSPSolver SHARED
	
	ReverseTSPSolver.h
	ReverseTSPSolver.cpp
)

set_target_properties(ReverseTSPSolver PROPERTIES FOLDER "Engine/Algorithms/TSPSolvers/Utilitary")

target_link_libraries(ReverseTSPSolver SceneManager SceneEditor)

cotire(ReverseTSPSolver)


GenerateExportHeader(ReverseTSPSolver)

InstallAlgorithm(ReverseTSPSolver)
InstallHeaders(ReverseTSPSolver.h)