add_library(OneRelocateTSPSolver SHARED
	
	OneRelocateTSPSolver.h
	OneRelocateTSPSolver.cpp
)

set_target_properties(OneRelocateTSPSolver PROPERTIES FOLDER "Engine/Algorithms/TSPSolvers/OneRelocateExperimental")

target_link_libraries(OneRelocateTSPSolver SceneManager SceneEditor)

cotire(OneRelocateTSPSolver)

GenerateExportHeader(OneRelocateTSPSolver)
InstallAlgorithm(OneRelocateTSPSolver)
InstallHeaders(OneRelocateTSPSolver.h)