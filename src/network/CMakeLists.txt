add_library(network network.cpp)
add_library(networkControls networkControls.cpp)
target_link_libraries(networkControls network)


