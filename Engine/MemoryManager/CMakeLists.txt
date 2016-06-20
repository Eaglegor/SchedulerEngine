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

include(GenerateExportHeader)
generate_export_header(MemoryManager EXPORT_FILE_NAME ${CPP_SCHEDULER_BINARY_DIR}/SharedLibrariesExportHeaders/MemoryManager_export.h)

if(CPP_SCHEDULER_TESTS)
	add_executable(MemoryManager_Test 
		Test/MemoryManager_Test.cpp
	)
		
	add_test(NAME MemoryManager_Test COMMAND $<TARGET_FILE:MemoryManager_Test>)
	target_link_libraries(MemoryManager_Test MemoryManager)
	set_target_properties(MemoryManager_Test PROPERTIES FOLDER "Engine/Tests/")
endif()
