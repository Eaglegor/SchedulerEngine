add_library(LoggingManager SHARED
    LoggingManager.h LoggingManager.cpp
	Renderer.h
)

set_target_properties(LoggingManager PROPERTIES FOLDER "Engine")

cotire(LoggingManager)

include(GenerateExportHeader)
generate_export_header(LoggingManager EXPORT_FILE_NAME ${CPP_SCHEDULER_BINARY_DIR}/SharedLibrariesExportHeaders/LoggingManager_export.h)

InstallCoreComponent(LoggingManager)
InstallHeaders(
    LoggingManager.h
    Renderer.h
)