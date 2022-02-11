add_executable(simple_test simple_test.cc)
target_link_libraries(simple_test gtest glog gflags) 
add_test(NAME simple_test COMMAND simple_test)
add_custom_target(simple_test_custom COMMAND ${CMAKE_CTEST_COMMAND} DEPENDS simple_test)
