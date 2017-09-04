add_library(CWSavingsVRPSolver SHARED
	
	CWSavingsVRPSolver.h CWSavingsVRPSolver.cpp
	
	Generators/ClassicSavingsGenerator.h
	
	Savings/ClassicSaving.h
	
	SavingsStores/VectorSavingsStore.h
)

set_target_properties(CWSavingsVRPSolver PROPERTIES FOLDER "Engine/Algorithms/VRPSolvers/CWSavings")

target_link_libraries(CWSavingsVRPSolver AlgorithmsManager SceneManager SceneEditor)

cotire(CWSavingsVRPSolver)




InstallAlgorithm(CWSavingsVRPSolver)
InstallHeaders(CWSavingsVRPSolver.h)