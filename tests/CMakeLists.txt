add_executable(joystick_test joystick_test.cpp)
target_link_libraries(joystick_test ${MODULES}) 
add_test(NAME joystick_test COMMAND joystick_test)
#add_custom_target(joystick_test_custom COMMAND ${CMAKE_CTEST_COMMAND} DEPENDS joystick_test)

add_executable(tof_sensor_test tof_sensor_test.cpp)
target_link_libraries(tof_sensor_test distance_sensors common) 
add_test(NAME tof_sensor_test COMMAND tof_sensor_test)

add_executable(wifi_test wifi_test.cpp)
target_link_libraries(wifi_test iw wireless) 
add_test(NAME wifi_test COMMAND wifi_test)

add_executable(network_test network_test.cpp)
target_link_libraries(network_test network rover) 
add_test(NAME network_test COMMAND network_test)

add_executable(network_controls_test network_controls_test.cpp)
target_link_libraries(network_controls_test networkControls network rover) 
add_test(NAME network_controls_test COMMAND network_controls_test)

add_executable(magnetometer_test magnetometer_test.cpp)
target_link_libraries(magnetometer_test magnetometer common) 
add_test(NAME magnetometer_test COMMAND magnetometer_test)
