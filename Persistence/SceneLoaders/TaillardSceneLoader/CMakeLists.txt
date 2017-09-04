add_library(TaillardSceneLoader SHARED
		TaillardSceneLoader.h	TaillardSceneLoader.cpp
)

target_link_libraries(TaillardSceneLoader Core)

set_target_properties(TaillardSceneLoader PROPERTIES FOLDER "Persistence/SceneLoaders")

cotire(TaillardSceneLoader)

include(GenerateExportHeader)
generate_export_header(TaillardSceneLoader EXPORT_FILE_NAME ${CPP_SCHEDULER_BINARY_DIR}/SharedLibrariesExportHeaders/TaillardSceneLoader_export.h)

InstallSerializer(TaillardSceneLoader)
InstallHeaders(TaillardSceneLoader.h)