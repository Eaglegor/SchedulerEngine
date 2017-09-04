macro(InstallHeaders)
    foreach(header ${ARGN})
		get_filename_component(DIR "${CMAKE_CURRENT_SOURCE_DIR}/${header}" DIRECTORY)
        file(RELATIVE_PATH REL_DIR ${CPP_SCHEDULER_SOURCE_DIR} ${DIR})
        install(FILES ${header} DESTINATION include/${REL_DIR})
    endforeach()
endmacro()

macro(InstallAlgorithm TARGET_NAME)
    install(TARGETS ${TARGET_NAME}
            LIBRARY DESTINATION ${CMAKE_BUILD_TYPE}/lib
            ARCHIVE DESTINATION ${CMAKE_BUILD_TYPE}/lib
            RUNTIME DESTINATION ${CMAKE_BUILD_TYPE}/lib
    )

    install(TARGETS ${TARGET_NAME}
            LIBRARY DESTINATION ${CMAKE_BUILD_TYPE}/test
            RUNTIME DESTINATION ${CMAKE_BUILD_TYPE}/test
    )

    install(TARGETS ${TARGET_NAME}
            LIBRARY DESTINATION ${CMAKE_BUILD_TYPE}/benchmark
            RUNTIME DESTINATION ${CMAKE_BUILD_TYPE}/benchmark
    )

    install(TARGETS ${TARGET_NAME}
            LIBRARY DESTINATION ${CMAKE_BUILD_TYPE}/tools
            RUNTIME DESTINATION ${CMAKE_BUILD_TYPE}/tools
    )
	
	install(FILES $<TARGET_PDB_FILE:${TARGET_NAME}> DESTINATION ${CMAKE_BUILD_TYPE}/lib OPTIONAL)
	install(FILES $<TARGET_PDB_FILE:${TARGET_NAME}> DESTINATION ${CMAKE_BUILD_TYPE}/test OPTIONAL)
	install(FILES $<TARGET_PDB_FILE:${TARGET_NAME}> DESTINATION ${CMAKE_BUILD_TYPE}/benchmark OPTIONAL)
	install(FILES $<TARGET_PDB_FILE:${TARGET_NAME}> DESTINATION ${CMAKE_BUILD_TYPE}/tools OPTIONAL)
endmacro()

macro(InstallCoreComponent TARGET_NAME)
    InstallAlgorithm(${TARGET_NAME})
endmacro()

macro(InstallCostFunction TARGET_NAME)
    InstallAlgorithm(${TARGET_NAME})
endmacro()

macro(InstallSerializer TARGET_NAME)
    InstallAlgorithm(${TARGET_NAME})
endmacro()

macro(InstallRoutingService TARGET_NAME)
    InstallAlgorithm(${TARGET_NAME})
endmacro()

macro(InstallDebugRenderer TARGET_NAME)
    InstallAlgorithm(${TARGET_NAME})
endmacro()

macro(InstallBenchmark TARGET_NAME)
    install(TARGETS ${TARGET_NAME}
        RUNTIME DESTINATION ${CMAKE_BUILD_TYPE}/benchmark
    )
	
	install(FILES $<TARGET_PDB_FILE:${TARGET_NAME}> DESTINATION ${CMAKE_BUILD_TYPE}/benchmark OPTIONAL)
endmacro()

macro(InstallTool TARGET_NAME)
    install(TARGETS ${TARGET_NAME}
        RUNTIME DESTINATION ${CMAKE_BUILD_TYPE}/tools
    )
	
	install(FILES $<TARGET_PDB_FILE:${TARGET_NAME}> DESTINATION ${CMAKE_BUILD_TYPE}/tools OPTIONAL)
endmacro()

macro(InstallTest TARGET_NAME)
    install(TARGETS ${TARGET_NAME}
        RUNTIME DESTINATION ${CMAKE_BUILD_TYPE}/test
    )
	
	install(FILES $<TARGET_PDB_FILE:${TARGET_NAME}> DESTINATION ${CMAKE_BUILD_TYPE}/test OPTIONAL)
endmacro()
