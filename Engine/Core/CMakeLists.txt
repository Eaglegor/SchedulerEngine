add_library(Engine SHARED
	
	Engine.h
	Engine.cpp
	
	Services/RoutingService.h
)

target_link_libraries(Engine 
    SceneManager 
	SceneEditor
    AlgorithmsManager
    MemoryManager
)

set_target_properties(Engine PROPERTIES FOLDER "Engine")

cotire(Engine)

include(GenerateExportHeader)
generate_export_header(Engine EXPORT_FILE_NAME ${CPP_SCHEDULER_BINARY_DIR}/SharedLibrariesExportHeaders/Engine_export.h)
