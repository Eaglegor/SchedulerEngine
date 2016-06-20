add_executable(UFTest_MemoryManager UFTest_MemoryManager.cpp)
add_test(NAME UFTest_MemoryManager COMMAND $<TARGET_FILE:UFTest_MemoryManager> WORKING_DIRECTORY ${EXECUTABLE_OUTPUT_PATH})
target_link_libraries(UFTest_MemoryManager MemoryManager)
set_target_properties(UFTest_MemoryManager PROPERTIES FOLDER "Tests/Engine/MemoryManager")

