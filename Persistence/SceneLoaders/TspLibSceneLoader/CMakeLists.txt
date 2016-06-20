add_library(TspLibSceneLoader SHARED
		TspLibSceneLoader.h	TspLibSceneLoader.cpp
)

target_link_libraries(TspLibSceneLoader Engine TspLibRoutingService)

set_target_properties(TspLibSceneLoader PROPERTIES FOLDER "Persistence/SceneLoaders")

cotire(TspLibSceneLoader)

include(GenerateExportHeader)
generate_export_header(TspLibSceneLoader EXPORT_FILE_NAME ${CPP_SCHEDULER_BINARY_DIR}/SharedLibrariesExportHeaders/TspLibSceneLoader_export.h)

if(CPP_SCHEDULER_TESTS)
	add_executable(TspLibSceneLoader_Test 
		Test/TspLibSceneLoader_Test.cpp
	)
	
	set_property(SOURCE Test/TspLibSceneLoader_Test.cpp PROPERTY COMPILE_DEFINITIONS "CATCH_CONFIG_MAIN")
		
	add_test(NAME TspLibSceneLoader_Test COMMAND $<TARGET_FILE:TspLibSceneLoader_Test>)
	target_link_libraries(TspLibSceneLoader_Test TspLibSceneLoader CrowFlyRoutingService)
	set_target_properties(TspLibSceneLoader_Test PROPERTIES FOLDER "Persistence/SceneLoaders/Tests/")
	
    add_custom_command(TARGET TspLibSceneLoader_Test POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_CURRENT_SOURCE_DIR}/Test/br17.xml ${EXECUTABLE_OUTPUT_PATH}/TestData/TspLibSceneLoader_Test/br17.xml)
    add_custom_command(TARGET TspLibSceneLoader_Test POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_CURRENT_SOURCE_DIR}/Test/br17.bin ${EXECUTABLE_OUTPUT_PATH}/TestData/TspLibSceneLoader_Test/br17.bin)

endif()