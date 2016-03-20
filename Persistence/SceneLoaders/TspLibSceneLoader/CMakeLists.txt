add_library(TspLibSceneLoader SHARED
		TspLibSceneLoader.h	TspLibSceneLoader.cpp
)

target_link_libraries(TspLibSceneLoader Engine TspLibRoutingService)

set_target_properties(TspLibSceneLoader PROPERTIES FOLDER "Persistence/SceneLoaders")

cotire(TspLibSceneLoader)

include(GenerateExportHeader)
generate_export_header(TspLibSceneLoader EXPORT_FILE_NAME ${CPP_SCHEDULER_BINARY_DIR}/SharedLibrariesExportHeaders/TspLibSceneLoader_export.h)
