add_executable(UFTest_JSONSceneLoader UFTest_JSONSceneLoader.cpp)
FILE(COPY TestScene1.json DESTINATION ${EXECUTABLE_OUTPUT_PATH})

ADD_CUSTOM_COMMAND(TARGET UFTest_JSONSceneLoader POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_CURRENT_SOURCE_DIR}/TestScene1.json ${EXECUTABLE_OUTPUT_PATH}/TestData/UFTest_JSONSceneLoader/TestScene1.json)
	
add_test(NAME UFTest_JSONSceneLoader COMMAND $<TARGET_FILE:UFTest_JSONSceneLoader> ${EXECUTABLE_OUTPUT_PATH}/TestData/UFTest_JSONSceneLoader/TestScene1.json)
set_target_properties(UFTest_JSONSceneLoader PROPERTIES FOLDER "Tests/SceneLoaders")