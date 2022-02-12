
# Format for Libraries:
# add_library(<module_name> <module_name>.cc)
# target_link_libraries(<module_name> <list of module_names that this module depends on>)

# Format for execuatables
# add_executable(main main.cpp)
# add_test(NAME main COMMAND main)
# add_custom_target(main_custom COMMAND ${CMAKE_CTEST_COMMAND} DEPENDS main)

#Directories
add_subdirectory(joystick)
add_subdirectory(led)
add_subdirectory(rover)
add_subdirectory(common)


#add_library(joystick STATIC joystick/joystick.cpp joystick/joystick.h)
#add_library(rover STATIC rover/rover.cpp rover/rover.h)

#target_link_libraries(rover)
