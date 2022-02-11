
# Format for Libraries:
# add_library(<module_name> <module_name>.cc)
# target_link_libraries(<module_name> <list of module_names that this module depends on>)

add_library(utils utils.cpp)
target_link_libraries(utils)

# Format for execuatables
add_executable(main main.cpp)
target_link_libraries(main pthread) 
add_test(NAME main COMMAND main)
add_custom_target(main_custom COMMAND ${CMAKE_CTEST_COMMAND} DEPENDS main)

#Directories
add_subdirectory(Joysticks)
add_subdirectory(Led)


