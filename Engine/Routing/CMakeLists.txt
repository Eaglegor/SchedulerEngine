add_custom_target(Routing SOURCES RoutingService.h)

set_target_properties(Routing PROPERTIES FOLDER "Engine")

InstallHeaders(
    RoutingService.h
)