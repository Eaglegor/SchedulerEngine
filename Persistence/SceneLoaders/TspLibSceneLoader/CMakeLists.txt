add_library(TspLibSceneLoader SHARED
		TspLibSceneLoader.h	TspLibSceneLoader.cpp
)

target_link_libraries(TspLibSceneLoader Core TspLibRoutingService)

set_target_properties(TspLibSceneLoader PROPERTIES FOLDER "Persistence/SceneLoaders")

cotire(TspLibSceneLoader)


GenerateExportHeader(TspLibSceneLoader)

InstallSerializer(TspLibSceneLoader)
InstallHeaders(TspLibSceneLoader.h)