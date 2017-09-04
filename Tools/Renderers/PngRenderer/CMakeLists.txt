add_library(PngRenderer SHARED
	PngRenderer.h PngRenderer.cpp
)

set_target_properties(PngRenderer PROPERTIES FOLDER "Tools/Renderers")

FIND_PACKAGE(Boost COMPONENTS REQUIRED filesystem)
target_link_libraries(PngRenderer ${Boost_LIBRARIES})

target_link_libraries(PngRenderer SceneManager)

cotire(PngRenderer)


GenerateExportHeader(PngRenderer)

InstallDebugRenderer(PngRenderer)
InstallHeaders(PngRenderer.h)