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

FIND_PACKAGE(Boost COMPONENTS REQUIRED date_time)
IF(Boost_FOUND)
	target_link_libraries(JSONSceneLoader ${Boost_LIBRARIES})
ENDIF()

target_link_libraries(JSONSceneLoader SchedulerEngine)

get_filename_component(DLL ${Boost_DATE_TIME_LIBRARY_DEBUG} NAME)

string(REGEX MATCH "^(.*)\\.[^.]*$" temp ${DLL})
set(BOOST_STRIPPED_LIBRARY_NAME ${CMAKE_MATCH_1})

if(WIN32)
	add_custom_command(TARGET JSONSceneLoader POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy ${BOOST_ROOT}/bin/${BOOST_STRIPPED_LIBRARY_NAME}.dll ${EXECUTABLE_OUTPUT_PATH})
endif()

include(GenerateExportHeader)
generate_export_header(JSONSceneLoader EXPORT_FILE_NAME ${CPP_SCHEDULER_BINARY_DIR}/SharedLibrariesExportHeaders/JSONSceneLoader_export.h)
