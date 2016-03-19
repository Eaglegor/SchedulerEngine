add_library(SceneEditor SHARED
	
	SceneEditor.h SceneEditor.cpp
    Action.h
    Checkpoint.h Checkpoint.cpp
    
    Actions/SwapRunWorkStops.h Actions/SwapRunWorkStops.cpp
    
)

set_target_properties(SceneEditor PROPERTIES FOLDER "Engine")

target_link_libraries(SceneEditor SceneManager)

cotire(SceneEditor)

include(GenerateExportHeader)
generate_export_header(SceneEditor EXPORT_FILE_NAME ${CPP_SCHEDULER_BINARY_DIR}/SharedLibrariesExportHeaders/SceneEditor_export.h)
