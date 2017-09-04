add_library(CrowFlyRoutingService SHARED CrowFlyRoutingService.h CrowFlyRoutingService.cpp)

target_link_libraries(CrowFlyRoutingService Core)

set_target_properties(CrowFlyRoutingService PROPERTIES FOLDER "Services/Routing")

cotire(CrowFlyRoutingService)


GenerateExportHeader(CrowFlyRoutingService)

InstallRoutingService(CrowFlyRoutingService)
InstallHeaders(CrowFlyRoutingService.h)