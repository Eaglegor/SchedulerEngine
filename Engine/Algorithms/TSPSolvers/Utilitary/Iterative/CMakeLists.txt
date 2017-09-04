add_library(IterativeTSPSolver SHARED
	
	IterativeTSPSolver.h
	IterativeTSPSolver.cpp
)

set_target_properties(IterativeTSPSolver PROPERTIES FOLDER "Engine/Algorithms/TSPSolvers/Utilitary")

target_link_libraries(IterativeTSPSolver SceneManager SceneEditor LoggingManager)

cotire(IterativeTSPSolver)


GenerateExportHeader(IterativeTSPSolver)

InstallAlgorithm(IterativeTSPSolver)
InstallHeaders(IterativeTSPSolver.h)