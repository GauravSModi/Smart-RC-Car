add_executable(joystick_test joystick_test.cpp)
target_link_libraries(joystick_test ${MODULES}) 
add_test(NAME joystick_test COMMAND joystick_test)
#add_custom_target(joystick_test_custom COMMAND ${CMAKE_CTEST_COMMAND} DEPENDS joystick_test)

