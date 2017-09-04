add_library(LoggingManager SHARED
    LoggingManager.h LoggingManager.cpp
	Renderer.h
)

set_target_properties(LoggingManager PROPERTIES FOLDER "Engine")

cotire(LoggingManager)


GenerateExportHeader(LoggingManager)

InstallCoreComponent(LoggingManager)
InstallHeaders(
    LoggingManager.h
    Renderer.h
)