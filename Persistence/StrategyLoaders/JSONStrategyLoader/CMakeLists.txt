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

target_link_libraries(JSONStrategyLoader Engine)

set_target_properties(JSONStrategyLoader PROPERTIES FOLDER "Persistence/StrategyLoaders")

cotire(JSONStrategyLoader)

include(GenerateExportHeader)
generate_export_header(JSONStrategyLoader EXPORT_FILE_NAME ${CPP_SCHEDULER_BINARY_DIR}/SharedLibrariesExportHeaders/JSONStrategyLoader_export.h)


if(CPP_SCHEDULER_TESTS)
	add_executable(JSONStrategyLoader_Test 
		Test/JSONStrategyLoader_Test.cpp
	)
	
	set_property(SOURCE Test/JSONStrategyLoader_Test.cpp PROPERTY COMPILE_DEFINITIONS "CATCH_CONFIG_MAIN")
		
	add_test(NAME JSONStrategyLoader_Test COMMAND $<TARGET_FILE:JSONStrategyLoader_Test> WORKING_DIRECTORY ${EXECUTABLE_OUTPUT_PATH})
	target_link_libraries(JSONStrategyLoader_Test JSONStrategyLoader CrowFlyRoutingService)
	set_target_properties(JSONStrategyLoader_Test PROPERTIES FOLDER "Persistence/StrategyLoaders/Tests/")
	
	add_custom_command(TARGET JSONStrategyLoader_Test POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_CURRENT_SOURCE_DIR}/Test/TestStrategy1.json ${EXECUTABLE_OUTPUT_PATH}/TestData/JSONStrategyLoader_Test/TestStrategy1.json)
	
	cotire(JSONStrategyLoader_Test)
endif()