add_executable(TspLibFormatConverter TspLibFormatConverter)

find_package(LibXml2)

if(LIBXML2_FOUND)
    target_link_libraries(TspLibFormatConverter ${LIBXML2_LIBRARIES})
endif()

find_package(Boost REQUIRED COMPONENTS program_options)
if(Boost_FOUND)
    target_link_libraries(TspLibFormatConverter ${Boost_LIBRARIES})
endif()

if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    get_filename_component(PODLL ${Boost_PROGRAM_OPTIONS_LIBRARY_DEBUG} NAME)
else()
    get_filename_component(PODLL ${Boost_PROGRAM_OPTIONS_LIBRARY_RELEASE} NAME)
endif()

string(REGEX MATCH "^(.*)\\.[^.]*$" temp ${PODLL})
set(BOOST_STRIPPED_POLIBRARY_NAME ${CMAKE_MATCH_1})

if(WIN32)
	add_custom_command(TARGET TspLibFormatConverter POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy ${BOOST_ROOT}/bin/${BOOST_STRIPPED_POLIBRARY_NAME}.dll ${EXECUTABLE_OUTPUT_PATH})
endif()

set_target_properties(TspLibFormatConverter PROPERTIES FOLDER "Tools")