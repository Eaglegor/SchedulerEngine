add_library(ScrambleTSPSolver SHARED
	
	ScrambleTSPSolver.h
	ScrambleTSPSolver.cpp
)

set_target_properties(ScrambleTSPSolver PROPERTIES FOLDER "Engine/Algorithms/TSPSolvers/Utilitary")

target_link_libraries(ScrambleTSPSolver SceneManager SceneEditor)

cotire(ScrambleTSPSolver)


GenerateExportHeader(ScrambleTSPSolver)

InstallAlgorithm(ScrambleTSPSolver)
InstallHeaders(ScrambleTSPSolver.h)