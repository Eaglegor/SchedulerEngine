add_library(TaillardSceneLoader SHARED
		TaillardSceneLoader.h	TaillardSceneLoader.cpp
)

target_link_libraries(TaillardSceneLoader Core)

set_target_properties(TaillardSceneLoader PROPERTIES FOLDER "Persistence/SceneLoaders")

cotire(TaillardSceneLoader)




InstallSerializer(TaillardSceneLoader)
InstallHeaders(TaillardSceneLoader.h)