add_library(TspLibRoutingService SHARED
    TspLibRoutingService.h TspLibRoutingService.cpp
)

target_link_libraries(TspLibRoutingService Concepts)

set_target_properties(TspLibRoutingService PROPERTIES FOLDER "Services/Routing")

cotire(TspLibRoutingService)


GenerateExportHeader(TspLibRoutingService)

InstallRoutingService(TspLibRoutingService)
InstallHeaders(TspLibRoutingService.h)