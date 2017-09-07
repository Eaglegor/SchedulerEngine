add_library(HybridOptTSPSolver SHARED
	
	HybridOptTSPSolver.h
	HybridOptTSPSolver.cpp
)

set_target_properties(HybridOptTSPSolver PROPERTIES FOLDER "Engine/Algorithms/TSPSolvers/HybridOpt")

target_link_libraries(HybridOptTSPSolver SceneManager SceneEditor)

cotire(HybridOptTSPSolver)


InstallAlgorithm(HybridOptTSPSolver)
InstallHeaders(HybridOptTSPSolver.h)