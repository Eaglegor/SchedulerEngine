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

find_package(Boost REQUIRED COMPONENTS system thread)
target_link_libraries(MemoryManager ${Boost_LIBRARIES})

if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    get_filename_component(DLL ${Boost_SYSTEM_LIBRARY_DEBUG} NAME)
else()
    get_filename_component(DLL ${Boost_SYSTEM_LIBRARY_RELEASE} NAME)
endif()

string(REGEX MATCH "^(.*)\\.[^.]*$" temp ${DLL})
set(BOOST_STRIPPED_LIBRARY_NAME ${CMAKE_MATCH_1})

if(WIN32)
	install(FILES ${BOOST_ROOT}/lib/${BOOST_STRIPPED_LIBRARY_NAME}.dll DESTINATION ${CMAKE_BUILD_TYPE}/lib)
endif()

include(GenerateExportHeader)
generate_export_header(MemoryManager EXPORT_FILE_NAME ${CPP_SCHEDULER_BINARY_DIR}/SharedLibrariesExportHeaders/MemoryManager_export.h)

InstallCoreComponent(MemoryManager)
InstallHeaders(
    MemoryManager.h
    MemoryStore.h
    MallocAllocator.h
    MallocMemoryStore.h
    PoolMemoryStore.h
    ObjectSizes.h
)