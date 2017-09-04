add_library(AlgorithmsManager SHARED
	AlgorithmsManager.h
	AlgorithmsManager.cpp
	
	Algorithm.h
	CostFunction.h
	
	TSPSolver.h
	VRPSolver.h
)

set_target_properties(AlgorithmsManager PROPERTIES FOLDER "Engine")

target_link_libraries(AlgorithmsManager Concepts MemoryManager)

cotire(AlgorithmsManager)




InstallCoreComponent(AlgorithmsManager)
InstallHeaders(
	AlgorithmsManager.h
	Algorithm.h
	CostFunction.h
	TSPSolver.h
	VRPSolver.h
)