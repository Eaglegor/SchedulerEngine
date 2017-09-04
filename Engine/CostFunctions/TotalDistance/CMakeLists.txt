add_library(TotalDistanceCostFunctions SHARED 
    TotalDistanceRunCostFunction.h TotalDistanceRunCostFunction.cpp
    TotalDistanceScheduleCostFunction.h TotalDistanceScheduleCostFunction.cpp
    TotalDistanceSceneCostFunction.h TotalDistanceSceneCostFunction.cpp
)

TARGET_LINK_LIBRARIES(TotalDistanceCostFunctions SceneManager)

set_target_properties(TotalDistanceCostFunctions PROPERTIES FOLDER "Engine/CostFunctions")

cotire(TotalDistanceCostFunctions)

include(GenerateExportHeader)
generate_export_header(TotalDistanceCostFunctions EXPORT_FILE_NAME ${CPP_SCHEDULER_BINARY_DIR}/SharedLibrariesExportHeaders/TotalDistanceCostFunctions_export.h)

InstallCostFunction(TotalDistanceCostFunctions)
InstallHeaders(
    TotalDistanceRunCostFunction.h
    TotalDistanceScheduleCostFunction.h
    TotalDistanceSceneCostFunction.h
)