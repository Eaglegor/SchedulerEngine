add_executable(UFTest_DurationUnits UFTest_DurationUnits.cpp)
add_test(NAME UFTest_DurationUnits COMMAND $<TARGET_FILE:UFTest_DurationUnits>)
target_link_libraries(UFTest_DurationUnits SchedulerEngine)
set_target_properties(UFTest_DurationUnits PROPERTIES FOLDER "Tests")

add_executable(UFTest_SpeedUnits UFTest_SpeedUnits.cpp)
add_test(NAME UFTest_SpeedUnits COMMAND $<TARGET_FILE:UFTest_SpeedUnits>)
target_link_libraries(UFTest_SpeedUnits SchedulerEngine)
set_target_properties(UFTest_SpeedUnits PROPERTIES FOLDER "Tests")

