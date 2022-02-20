add_library(rover rover.cpp)
# rover uses modules joystick,network,led
target_link_libraries(rover common joystick led network)

