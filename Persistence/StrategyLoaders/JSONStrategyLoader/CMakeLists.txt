add_library(JSONStrategyLoader SHARED
		JSONStrategyLoader.h JSONStrategyLoader.cpp
		LoaderImpl.h LoaderImpl.cpp
		
		Loaders/VRPSolvers/VRPSolverLoader.h
		Loaders/VRPSolvers/TransparentVRPSolverLoader.h Loaders/VRPSolvers/TransparentVRPSolverLoader.cpp
		Loaders/VRPSolvers/ChainVRPSolverLoader.h Loaders/VRPSolvers/ChainVRPSolverLoader.cpp
		Loaders/VRPSolvers/TSPOnlyVRPSolverLoader.h Loaders/VRPSolvers/TSPOnlyVRPSolverLoader.cpp
		
		Loaders/TSPSolvers/TSPSolverLoader.h
		Loaders/TSPSolvers/SimpleTwoOptTSPSolverLoader.h Loaders/TSPSolvers/SimpleTwoOptTSPSolverLoader.cpp
		
		Loaders/RunCostFunctions/RunCostFunctionLoader.h
		Loaders/RunCostFunctions/TotalDistanceRunCostFunctionLoader.h Loaders/RunCostFunctions/TotalDistanceRunCostFunctionLoader.cpp
		
		Loaders/ScheduleCostFunctions/ScheduleCostFunctionLoader.h
		Loaders/ScheduleCostFunctions/TotalDistanceScheduleCostFunctionLoader.h Loaders/ScheduleCostFunctions/TotalDistanceScheduleCostFunctionLoader.cpp
		
		Loaders/SceneCostFunctions/SceneCostFunctionLoader.h
		Loaders/SceneCostFunctions/TotalDistanceSceneCostFunctionLoader.h Loaders/SceneCostFunctions/TotalDistanceSceneCostFunctionLoader.cpp
)

target_link_libraries(JSONStrategyLoader Core)

set_target_properties(JSONStrategyLoader PROPERTIES FOLDER "Persistence/StrategyLoaders")

cotire(JSONStrategyLoader)




InstallSerializer(JSONStrategyLoader)
InstallHeaders(JSONStrategyLoader.h)