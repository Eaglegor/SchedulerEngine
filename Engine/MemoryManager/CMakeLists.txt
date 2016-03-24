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
