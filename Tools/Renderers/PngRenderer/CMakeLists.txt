add_library(PngRenderer SHARED
	PngRenderer.h PngRenderer.cpp
)

set_target_properties(PngRenderer PROPERTIES FOLDER "Tools/Renderers")

FIND_PACKAGE(Boost COMPONENTS REQUIRED filesystem)
IF(Boost_FOUND)
	target_link_libraries(PngRenderer ${Boost_LIBRARIES})
ENDIF()

target_link_libraries(PngRenderer SceneManager)

cotire(PngRenderer)

include(GenerateExportHeader)
generate_export_header(PngRenderer EXPORT_FILE_NAME ${CPP_SCHEDULER_BINARY_DIR}/SharedLibrariesExportHeaders/PngRenderer_export.h)
