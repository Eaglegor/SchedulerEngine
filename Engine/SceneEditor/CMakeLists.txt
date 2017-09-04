add_library(SceneEditor SHARED
	
	SceneEditor.h SceneEditor.cpp
    Action.h
    Checkpoint.h Checkpoint.cpp
    Patch.h Patch.cpp
    Actions/SwapWorkStops.h Actions/SwapWorkStops.cpp
    Actions/ReverseWorkStops.h Actions/ReverseWorkStops.cpp
    Actions/MoveWorkStop.h Actions/MoveWorkStop.cpp
    Actions/ScrambleWorkStops.h Actions/ScrambleWorkStops.cpp
    Actions/RotateWorkStops.h Actions/RotateWorkStops.cpp
	Actions/MoveWorkStops.h Actions/MoveWorkStops.cpp
	Actions/AllocateOrder.h Actions/AllocateOrder.cpp
	Actions/CreateRun.h Actions/CreateRun.cpp
	Actions/DestroyRun.h Actions/DestroyRun.cpp
)

set_target_properties(SceneEditor PROPERTIES FOLDER "Engine")

target_link_libraries(SceneEditor SceneManager LoggingManager)

cotire(SceneEditor)


GenerateExportHeader(SceneEditor)

InstallCoreComponent(SceneEditor)
InstallHeaders(
    SceneEditor.h
    Action.h
    Checkpoint.h
    Patch.h
    Actions/SwapWorkStops.h
    Actions/ReverseWorkStops.h
    Actions/MoveWorkStop.h
    Actions/ScrambleWorkStops.h
    Actions/RotateWorkStops.h
    Actions/MoveWorkStops.h
    Actions/AllocateOrder.h
    Actions/CreateRun.h
    Actions/DestroyRun.h
)