add_executable(PMTest_Boost PMTest_Boost.cpp)

find_package(Boost COMPONENTS program_options)

target_link_libraries(PMTest_Boost ${Boost_LIBRARIES})