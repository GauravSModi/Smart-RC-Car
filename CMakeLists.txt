cmake_minimum_required(VERSION 3.10)

# Parameter Variables
# ======================================
# modules main requies to work
set(MODULES ${MODULES} rover common joystick network led gyro accelerometer)
# enable testing [YES/NO]
if (EXISTS "./.enableTest")
  set(ENABLE_TEST YES)
else()
  set(ENABLE_TEST NO)
endif()

# ======================================
# toolchain to specify compiler
set(CMAKE_TOOLCHAIN_FILE toolchain/toolchain-arm-linux-gnueabihf.cmake)

project(SMART-RC-CAR
        LANGUAGES CXX)

# in-source build prevention and Debug mode default adapted from 
# No in-source build
if("${CMAKE_SOURCE_DIR}" STREQUAL "${CMAKE_BINARY_DIR}")
  message(FATAL_ERROR "In-source builds not allowed.")
endif("${CMAKE_SOURCE_DIR}" STREQUAL "${CMAKE_BINARY_DIR}")

# Choose Debug mode by default
if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE "Debug" CACHE STRING "" FORCE)
endif(NOT CMAKE_BUILD_TYPE)

set(CMAKE_CXX_STANDARD 11)
#set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Werror -pthread -liw")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pthread -liw")

#find package
#find_package( OpenCV REQUIRED )

#include_directories()
INCLUDE_DIRECTORIES("${CMAKE_SOURCE_DIR}/src")
INCLUDE_DIRECTORIES("${CMAKE_SOURCE_DIR}/src/camera/")
LINK_DIRECTORIES("${CMAKE_SOURCE_DIR}/lib-multiarch")

# Module Directories
add_subdirectory(src)
add_subdirectory(lib-multiarch)
include_directories( ${OpenCV_INCLUDE_DIRS} )

# Test Directories

# build main executable
add_executable(main src/main.cpp)
target_link_libraries(main ${MODULES})

# Testing framework
if(ENABLE_TEST)
  add_subdirectory(tests)
endif()

