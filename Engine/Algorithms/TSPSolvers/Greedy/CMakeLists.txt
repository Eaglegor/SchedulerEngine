add_library(GreedyTSPSolver SHARED
	
	GreedyTSPSolver.h
	GreedyTSPSolver.cpp
)

set_target_properties(GreedyTSPSolver PROPERTIES FOLDER "Engine/Algorithms/TSPSolvers/Greedy")

target_link_libraries(GreedyTSPSolver SceneManager SceneEditor LoggingManager)

cotire(GreedyTSPSolver)

GenerateExportHeader(GreedyTSPSolver)
InstallAlgorithm(GreedyTSPSolver)
InstallHeaders(GreedyTSPSolver.h)
