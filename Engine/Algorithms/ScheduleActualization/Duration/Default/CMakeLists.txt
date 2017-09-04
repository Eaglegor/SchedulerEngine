add_library(DefaultDurationActualizationAlgorithm SHARED
	
	DefaultDurationActualizationAlgorithm.h
	DefaultDurationActualizationAlgorithm.cpp
)

set_target_properties(DefaultDurationActualizationAlgorithm PROPERTIES FOLDER "Engine/Algorithms/ScheduleActualization/Duration")

target_link_libraries(DefaultDurationActualizationAlgorithm SceneManager)

cotire(DefaultDurationActualizationAlgorithm)

include(GenerateExportHeader)
generate_export_header(DefaultDurationActualizationAlgorithm EXPORT_FILE_NAME ${CPP_SCHEDULER_BINARY_DIR}/SharedLibrariesExportHeaders/DefaultDurationActualizationAlgorithm_export.h)

install(TARGETS DefaultDurationActualizationAlgorithm
		LIBRARY DESTINATION ${CMAKE_BUILD_TYPE}/lib
		ARCHIVE DESTINATION ${CMAKE_BUILD_TYPE}/lib
		RUNTIME DESTINATION ${CMAKE_BUILD_TYPE}/lib
	)

InstallHeaders(DefaultDurationActualizationAlgorithm.h)