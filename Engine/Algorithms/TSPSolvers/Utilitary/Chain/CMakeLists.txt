add_library(ChainTSPSolver SHARED
	
	ChainTSPSolver.h
	ChainTSPSolver.cpp
)

set_target_properties(ChainTSPSolver PROPERTIES FOLDER "Engine/Algorithms/TSPSolvers/Utilitary")

target_link_libraries(ChainTSPSolver SceneManager SceneEditor)

cotire(ChainTSPSolver)




InstallAlgorithm(ChainTSPSolver)
InstallHeaders(ChainTSPSolver.h)