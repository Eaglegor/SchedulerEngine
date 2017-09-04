add_library(SimpleTwoOptTSPSolver SHARED
	
	SimpleTwoOptTSPSolver.h
	SimpleTwoOptTSPSolver.cpp
)

set_target_properties(SimpleTwoOptTSPSolver PROPERTIES FOLDER "Engine/Algorithms/TSPSolvers/SimpleTwoOpt")

target_link_libraries(SimpleTwoOptTSPSolver SceneManager SceneEditor)

cotire(SimpleTwoOptTSPSolver)

GenerateExportHeader(SimpleTwoOptTSPSolver)

InstallAlgorithm(SimpleTwoOptTSPSolver)
InstallHeaders(SimpleTwoOptTSPSolver.h)