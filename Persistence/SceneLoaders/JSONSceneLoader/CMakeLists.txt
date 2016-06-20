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
IF(Boost_FOUND)
	target_link_libraries(JSONSceneLoader ${Boost_LIBRARIES})
ENDIF()

target_link_libraries(JSONSceneLoader Engine)

if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    get_filename_component(DLL ${Boost_DATE_TIME_LIBRARY_DEBUG} NAME)
else()
    get_filename_component(DLL ${Boost_DATE_TIME_LIBRARY_RELEASE} NAME)
endif()

string(REGEX MATCH "^(.*)\\.[^.]*$" temp ${DLL})
set(BOOST_STRIPPED_LIBRARY_NAME ${CMAKE_MATCH_1})

if(WIN32)
	add_custom_command(TARGET JSONSceneLoader POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy ${BOOST_ROOT}/bin/${BOOST_STRIPPED_LIBRARY_NAME}.dll ${EXECUTABLE_OUTPUT_PATH})
endif()

set_target_properties(JSONSceneLoader PROPERTIES FOLDER "Persistence/SceneLoaders")

cotire(JSONSceneLoader)

include(GenerateExportHeader)
generate_export_header(JSONSceneLoader EXPORT_FILE_NAME ${CPP_SCHEDULER_BINARY_DIR}/SharedLibrariesExportHeaders/JSONSceneLoader_export.h)


if(CPP_SCHEDULER_TESTS)
	add_executable(JSONSceneLoader_Test 
		Test/JSONSceneLoader_Test.cpp
		Test/JSONSceneLoader_Test_Deserializer.cpp
	)
	
	set_property(SOURCE Test/JSONSceneLoader_Test.cpp PROPERTY COMPILE_DEFINITIONS "CATCH_CONFIG_MAIN")
		
	add_test(NAME JSONSceneLoader_Test COMMAND $<TARGET_FILE:JSONSceneLoader_Test> WORKING_DIRECTORY ${EXECUTABLE_OUTPUT_PATH})
	target_link_libraries(JSONSceneLoader_Test JSONSceneLoader CrowFlyRoutingService)
	set_target_properties(JSONSceneLoader_Test PROPERTIES FOLDER "Persistence/SceneLoaders/Tests/")
	
	add_custom_command(TARGET JSONSceneLoader_Test POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_CURRENT_SOURCE_DIR}/Test/TestScene1.json ${EXECUTABLE_OUTPUT_PATH}/TestData/JSONSceneLoader_Test/TestScene1.json)
endif()