add_executable(UFTest_DurationUnits UFTest_DurationUnits.cpp)
add_test(NAME UFTest_DurationUnits COMMAND $<TARGET_FILE:UFTest_DurationUnits>)
target_link_libraries(UFTest_DurationUnits Engine)
set_target_properties(UFTest_DurationUnits PROPERTIES FOLDER "Tests/Engine/Utils/Units")
cotire(UFTest_DurationUnits)

add_executable(UFTest_SpeedUnits UFTest_SpeedUnits.cpp)
add_test(NAME UFTest_SpeedUnits COMMAND $<TARGET_FILE:UFTest_SpeedUnits>)
target_link_libraries(UFTest_SpeedUnits Engine)
set_target_properties(UFTest_SpeedUnits PROPERTIES FOLDER "Tests/Engine/Utils/Units")
cotire(UFTest_SpeedUnits)

