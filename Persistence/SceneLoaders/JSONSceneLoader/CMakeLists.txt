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
)

FIND_PACKAGE(Boost COMPONENTS REQUIRED date_time)
target_link_libraries(JSONSceneLoader ${Boost_LIBRARIES})

target_link_libraries(JSONSceneLoader Core)

if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    get_filename_component(DLL ${Boost_DATE_TIME_LIBRARY_DEBUG} NAME)
else()
    get_filename_component(DLL ${Boost_DATE_TIME_LIBRARY_RELEASE} NAME)
endif()

string(REGEX MATCH "^(.*)\\.[^.]*$" temp ${DLL})
set(BOOST_STRIPPED_LIBRARY_NAME ${CMAKE_MATCH_1})

if(WIN32)
	install(FILES ${BOOST_ROOT}/lib/${BOOST_STRIPPED_LIBRARY_NAME}.dll DESTINATION ${CMAKE_BUILD_TYPE}/lib)
endif()

set_target_properties(JSONSceneLoader PROPERTIES FOLDER "Persistence/SceneLoaders")

cotire(JSONSceneLoader)


GenerateExportHeader(JSONSceneLoader)

InstallSerializer(JSONSceneLoader)
InstallHeaders(JSONSceneLoader.h)