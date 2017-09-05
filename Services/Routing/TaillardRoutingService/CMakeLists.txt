add_library(TaillardRoutingService SHARED TaillardRoutingService.h TaillardRoutingService.cpp)

target_link_libraries(TaillardRoutingService Core)

set_target_properties(TaillardRoutingService PROPERTIES FOLDER "Services/Routing")

cotire(TaillardRoutingService)




InstallRoutingService(TaillardRoutingService)
InstallHeaders(TaillardRoutingService.h)