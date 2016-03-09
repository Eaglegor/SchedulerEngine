add_library(JSONSceneLoader SHARED
		JSONSceneLoader.h
		JSONSceneLoader.cpp
		
		Model/LoaderSettings.h
		Model/FleetDesc.h
		Model/LocationDesc.h
		Model/OperationDesc.h
		Model/OrderDesc.h
		Model/PerformerDesc.h
		Model/TimeWindowDesc.h
		Model/VehicleDesc.h
		Model/SceneDesc.h
		Model/RoutingProfileDesc.h
		Model/ScheduleDesc.h
		Model/ScheduleShiftDesc.h
		Model/RunDesc.h
		Model/StopDesc.h

		Model/PtreeDeserializer.h
)

target_link_libraries(JSONSceneLoader SchedulerEngine)

include(GenerateExportHeader)
generate_export_header(JSONSceneLoader EXPORT_FILE_NAME ${CPP_SCHEDULER_BINARY_DIR}/SharedLibrariesExportHeaders/JSONSceneLoader_export.h)