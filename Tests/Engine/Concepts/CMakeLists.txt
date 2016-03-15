add_executable(UFTest_Capacity UFTest_Capacity.cpp)
add_test(NAME UFTest_Capacity COMMAND $<TARGET_FILE:UFTest_Capacity>)
target_link_libraries(UFTest_Capacity Engine)
set_target_properties(UFTest_Capacity PROPERTIES FOLDER "Tests/Engine/Concepts")

add_executable(UFTest_Coordinate UFTest_Coordinate.cpp)
add_test(NAME UFTest_Coordinate COMMAND $<TARGET_FILE:UFTest_Coordinate>)
target_link_libraries(UFTest_Coordinate Engine)
set_target_properties(UFTest_Coordinate PROPERTIES FOLDER "Tests/Engine/Concepts")

add_executable(UFTest_Cost UFTest_Cost.cpp)
add_test(NAME UFTest_Cost COMMAND $<TARGET_FILE:UFTest_Cost>)
target_link_libraries(UFTest_Cost Engine)
set_target_properties(UFTest_Cost PROPERTIES FOLDER "Tests/Engine/Concepts")

add_executable(UFTest_Distance UFTest_Distance.cpp)
add_test(NAME UFTest_Distance COMMAND $<TARGET_FILE:UFTest_Distance>)
target_link_libraries(UFTest_Distance Engine)
set_target_properties(UFTest_Distance PROPERTIES FOLDER "Tests/Engine/Concepts")

add_executable(UFTest_Duration UFTest_Duration.cpp)
add_test(NAME UFTest_Duration COMMAND $<TARGET_FILE:UFTest_Duration>)
target_link_libraries(UFTest_Duration Engine)
set_target_properties(UFTest_Duration PROPERTIES FOLDER "Tests/Engine/Concepts")

add_executable(UFTest_Location UFTest_Location.cpp)
add_test(NAME UFTest_Location COMMAND $<TARGET_FILE:UFTest_Location>)
target_link_libraries(UFTest_Location Engine)
set_target_properties(UFTest_Location PROPERTIES FOLDER "Tests/Engine/Concepts")

add_executable(UFTest_Route UFTest_Route.cpp)
add_test(NAME UFTest_Route COMMAND $<TARGET_FILE:UFTest_Route>)
target_link_libraries(UFTest_Route Engine)
set_target_properties(UFTest_Route PROPERTIES FOLDER "Tests/Engine/Concepts")

add_executable(UFTest_TimePoint UFTest_TimePoint.cpp)
add_test(NAME UFTest_TimePoint COMMAND $<TARGET_FILE:UFTest_TimePoint>)
target_link_libraries(UFTest_TimePoint Engine)
set_target_properties(UFTest_TimePoint PROPERTIES FOLDER "Tests/Engine/Concepts")

add_executable(UFTest_RoutingProfile UFTest_RoutingProfile.cpp)
add_test(NAME UFTest_RoutingProfile COMMAND $<TARGET_FILE:UFTest_RoutingProfile>)
target_link_libraries(UFTest_RoutingProfile Engine)
set_target_properties(UFTest_RoutingProfile PROPERTIES FOLDER "Tests/Engine/Concepts")

add_executable(UFTest_Speed UFTest_Speed.cpp)
add_test(NAME UFTest_Speed COMMAND $<TARGET_FILE:UFTest_Speed>)
target_link_libraries(UFTest_Speed Engine)
set_target_properties(UFTest_Speed PROPERTIES FOLDER "Tests/Engine/Concepts")

add_executable(UFTest_TimeWindow UFTest_TimeWindow.cpp)
add_test(NAME UFTest_TimeWindow COMMAND $<TARGET_FILE:UFTest_TimeWindow>)
target_link_libraries(UFTest_TimeWindow Engine)
set_target_properties(UFTest_TimeWindow PROPERTIES FOLDER "Tests/Engine/Concepts")

add_executable(UFTest_SpeedDurationDistanceOperations UFTest_SpeedDurationDistanceOperations.cpp)
add_test(NAME UFTest_SpeedDurationDistanceOperations COMMAND $<TARGET_FILE:UFTest_SpeedDurationDistanceOperations>)
target_link_libraries(UFTest_SpeedDurationDistanceOperations Engine)
set_target_properties(UFTest_SpeedDurationDistanceOperations PROPERTIES FOLDER "Tests/Engine/Concepts")
