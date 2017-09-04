add_library(TspLibRoutingService SHARED
    TspLibRoutingService.h TspLibRoutingService.cpp
)

target_link_libraries(TspLibRoutingService Concepts)

set_target_properties(TspLibRoutingService PROPERTIES FOLDER "Services/Routing")

cotire(TspLibRoutingService)

include(GenerateExportHeader)
generate_export_header(TspLibRoutingService EXPORT_FILE_NAME ${CPP_SCHEDULER_BINARY_DIR}/SharedLibrariesExportHeaders/TspLibRoutingService_export.h)

InstallRoutingService(TspLibRoutingService)
InstallHeaders(TspLibRoutingService.h)