add_library(PngRenderer SHARED
	PngRenderer.h PngRenderer.cpp
)

set_target_properties(PngRenderer PROPERTIES FOLDER "Tools/Renderers")

find_package(PNG)

include_directories(${PNG_INCLUDE_DIRS})
#add_definitions(PngRenderer ${PNG_DEFINITIONS})
target_link_libraries(PngRenderer SceneManager ${PNG_LIBRARIES})

cotire(PngRenderer)

include(GenerateExportHeader)
generate_export_header(PngRenderer EXPORT_FILE_NAME ${CPP_SCHEDULER_BINARY_DIR}/SharedLibrariesExportHeaders/PngRenderer_export.h)
