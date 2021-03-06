add_library(SimulatedAnnealingTSPSolver SHARED
	
	SimulatedAnnealingTSPSolver.h
	SimulatedAnnealingTSPSolver.cpp
	TemperatureScheduler.h
	TemperatureScheduler.cpp
	DefaultTemperatureScheduler.h
	DefaultTemperatureScheduler.cpp
	ListTemperatureScheduler.h
	ListTemperatureScheduler.cpp
	FastListTemperatureScheduler.h
	FastListTemperatureScheduler.cpp
	SlowListTemperatureScheduler.h
	SlowListTemperatureScheduler.cpp
	SolutionGenerator.h
	SolutionGenerator.cpp
	SolutionGeneratorClassic.h
	SolutionGeneratorClassic.cpp
	InstanceBasedSolutionGenerator.h
    InstanceBasedSolutionGenerator.cpp
)

set_target_properties(SimulatedAnnealingTSPSolver PROPERTIES FOLDER "Engine/Algorithms/TSPSolvers/SimulatedAnnealing")

target_link_libraries(SimulatedAnnealingTSPSolver SceneManager SceneEditor)

cotire(SimulatedAnnealingTSPSolver)



InstallAlgorithm(SimulatedAnnealingTSPSolver)
InstallHeaders(
	SimulatedAnnealingTSPSolver.h
	TemperatureScheduler.h
	DefaultTemperatureScheduler.h
	ListTemperatureScheduler.h
	FastListTemperatureScheduler.h
	SlowListTemperatureScheduler.h
	SolutionGenerator.h
	SolutionGeneratorClassic.h
	InstanceBasedSolutionGenerator.h
)