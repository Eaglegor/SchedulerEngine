add_executable(BTest_Taillard BTest_Taillard.cpp)

#add_test(NAME BTest_Taillard COMMAND $<TARGET_FILE:BTest_Taillard>)

target_link_libraries(BTest_Taillard Engine EuclideanRoutingService TaillardSceneLoader)
set_target_properties(BTest_Taillard PROPERTIES FOLDER "Tests/Benchmark/Taillard")
add_definitions(-DTAILLARD_BENCHMARK_DATA_ROOT="${CPP_SCHEDULER_BENCHMARK_DATA_DIRECTORY}/Data/CVRP/Taillard")
