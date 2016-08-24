add_library(MemoryManager SHARED
    MemoryManager.h MemoryManager.cpp
	MemoryStore.h
    MallocAllocator.h
    MallocMemoryStore.h MallocMemoryStore.cpp
    PoolMemoryStore.h PoolMemoryStore.cpp
    ObjectSizes.h
)

set_target_properties(MemoryManager PROPERTIES FOLDER "Engine")

cotire(MemoryManager)

find_package(Boost REQUIRED COMPONENTS system)
target_link_libraries(MemoryManager ${Boost_LIBRARIES})

if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    get_filename_component(DLL ${Boost_SYSTEM_LIBRARY_DEBUG} NAME)
else()
    get_filename_component(DLL ${Boost_SYSTEM_LIBRARY_RELEASE} NAME)
endif()

string(REGEX MATCH "^(.*)\\.[^.]*$" temp ${DLL})
set(BOOST_STRIPPED_LIBRARY_NAME ${CMAKE_MATCH_1})

if(WIN32)
	add_custom_command(TARGET MemoryManager POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy ${BOOST_ROOT}/bin/${BOOST_STRIPPED_LIBRARY_NAME}.dll ${EXECUTABLE_OUTPUT_PATH})
endif()

include(GenerateExportHeader)
generate_export_header(MemoryManager EXPORT_FILE_NAME ${CPP_SCHEDULER_BINARY_DIR}/SharedLibrariesExportHeaders/MemoryManager_export.h)

if(CPP_SCHEDULER_TESTS)
	add_executable(MemoryManager_Test 
		Test/MemoryManager_Test.cpp
	)
		
	add_test(NAME MemoryManager_Test COMMAND $<TARGET_FILE:MemoryManager_Test>)
	target_link_libraries(MemoryManager_Test MemoryManager)
	set_target_properties(MemoryManager_Test PROPERTIES FOLDER "Engine/Tests/")
	
	cotire(MemoryManager_Test)
endif()
