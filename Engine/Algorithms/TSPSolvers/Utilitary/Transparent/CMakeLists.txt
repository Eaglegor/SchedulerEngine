add_library(TransparentTSPSolver SHARED
	
	TransparentTSPSolver.h
	TransparentTSPSolver.cpp
)

set_target_properties(TransparentTSPSolver PROPERTIES FOLDER "Engine/Algorithms/TSPSolvers/Utilitary")

target_link_libraries(TransparentTSPSolver SceneManager)

cotire(TransparentTSPSolver)


GenerateExportHeader(TransparentTSPSolver)

InstallAlgorithm(TransparentTSPSolver)
InstallHeaders(TransparentTSPSolver.h)