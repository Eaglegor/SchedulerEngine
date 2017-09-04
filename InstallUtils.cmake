macro(InstallHeaders HEADER_LIST)
    foreach(header ${HEADER_LIST})
        message(STATUS ${header})
        string(REGEX MATCH "(.\\\*)\\\[/\\\]" DIR ${header})
        file(RELATIVE_PATH REL_DIR ${CPP_SCHEDULER_SOURCE_DIR} ${CMAKE_CURRENT_SOURCE_DIR})
        install(FILES ${header} DESTINATION include/${REL_DIR}/${DIR})
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
endmacro()

macro(InstallTool TARGET_NAME)
    install(TARGETS ${TARGET_NAME}
        RUNTIME DESTINATION ${CMAKE_BUILD_TYPE}/tools
    )
endmacro()

macro(InstallTest TARGET_NAME)
    install(TARGETS ${TARGET_NAME}
        RUNTIME DESTINATION ${CMAKE_BUILD_TYPE}/test
    )
endmacro()

macro(GenerateExportHeader TARGET_NAME)
    include(GenerateExportHeader)
    generate_export_header(${TARGET_NAME} EXPORT_FILE_NAME ${CPP_SCHEDULER_BINARY_DIR}/ExportHeaders/${TARGET_NAME}_export.h)
    include_directories(${CPP_SCHEDULER_BINARY_DIR}/ExportHeaders)
    install(FILES ${TARGET_NAME}_EXPORT_HEADER DESTINATION ${CMAKE_BUILD_TYPE}/include)
endmacro()