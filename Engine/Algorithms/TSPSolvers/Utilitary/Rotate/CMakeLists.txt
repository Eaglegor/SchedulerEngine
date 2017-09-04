add_library(RotateTSPSolver SHARED
	
	RotateTSPSolver.h
	RotateTSPSolver.cpp
)

set_target_properties(RotateTSPSolver PROPERTIES FOLDER "Engine/Algorithms/TSPSolvers/Utilitary")

target_link_libraries(RotateTSPSolver SceneManager SceneEditor)

cotire(RotateTSPSolver)




InstallAlgorithm(RotateTSPSolver)
InstallHeaders(RotateTSPSolver.h)