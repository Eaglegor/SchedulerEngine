add_executable(TspLibFormatConverter TspLibFormatConverter)

find_package(LibXml2)

if(LIBXML2_FOUND)
    target_link_libraries(TspLibFormatConverter ${LIBXML2_LIBRARIES})
    include_directories(TspLibFormatConverter ${LIBXML2_ROOT}/include/libxml2)
endif()

find_package(Boost REQUIRED COMPONENTS program_options)
target_link_libraries(TspLibFormatConverter ${Boost_LIBRARIES})

if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    get_filename_component(PODLL ${Boost_PROGRAM_OPTIONS_LIBRARY_DEBUG} NAME)
else()
    get_filename_component(PODLL ${Boost_PROGRAM_OPTIONS_LIBRARY_RELEASE} NAME)
endif()

string(REGEX MATCH "^(.*)\\.[^.]*$" temp ${PODLL})
set(BOOST_STRIPPED_POLIBRARY_NAME ${CMAKE_MATCH_1})

install(TARGETS TspLibFormatConverter
        RUNTIME DESTINATION ${CMAKE_BUILD_TYPE}/tools
)

if(WIN32)
    install(FILES ${BOOST_ROOT}/lib/${BOOST_STRIPPED_POLIBRARY_NAME}.dll DESTINATION ${CMAKE_BUILD_TYPE}/tools)
endif()

set_target_properties(TspLibFormatConverter PROPERTIES FOLDER "Tools")