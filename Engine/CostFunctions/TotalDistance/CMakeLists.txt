add_library(TotalDistanceCostFunctions SHARED 
    TotalDistanceRunCostFunction.h TotalDistanceRunCostFunction.cpp
    TotalDistanceScheduleCostFunction.h TotalDistanceScheduleCostFunction.cpp
    TotalDistanceSceneCostFunction.h TotalDistanceSceneCostFunction.cpp
)

TARGET_LINK_LIBRARIES(TotalDistanceCostFunctions SceneManager)

set_target_properties(TotalDistanceCostFunctions PROPERTIES FOLDER "Engine/CostFunctions")

cotire(TotalDistanceCostFunctions)




InstallCostFunction(TotalDistanceCostFunctions)
InstallHeaders(
    TotalDistanceRunCostFunction.h
    TotalDistanceScheduleCostFunction.h
    TotalDistanceSceneCostFunction.h
)