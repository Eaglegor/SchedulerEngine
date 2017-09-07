add_library(BestOfTSPSolver SHARED
	
	BestOfTSPSolver.h
	BestOfTSPSolver.cpp
)

set_target_properties(BestOfTSPSolver PROPERTIES FOLDER "Engine/Algorithms/TSPSolvers/Utilitary")

target_link_libraries(BestOfTSPSolver SceneManager SceneEditor LoggingManager)

cotire(BestOfTSPSolver)




InstallAlgorithm(BestOfTSPSolver)
InstallHeaders(BestOfTSPSolver.h)