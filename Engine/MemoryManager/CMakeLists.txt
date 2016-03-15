add_library(MemoryManager SHARED
    MemoryManager.h
    MemoryManager.cpp
)

set_target_properties(MemoryManager PROPERTIES FOLDER "Engine")

include(GenerateExportHeader)
generate_export_header(MemoryManager EXPORT_FILE_NAME ${CPP_SCHEDULER_BINARY_DIR}/SharedLibrariesExportHeaders/MemoryManager_export.h)
