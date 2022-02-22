cmake_minimum_required(VERSION 3.10)

# Parameter Variables
# ======================================
# modules main requies to work
set(MODULES ${MODULES} rover common joystick network led)
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

# No in-source build
if("${CMAKE_SOURCE_DIR}" STREQUAL "${CMAKE_BINARY_DIR}")
  message(FATAL_ERROR "In-source builds are not allowed.")
endif("${CMAKE_SOURCE_DIR}" STREQUAL "${CMAKE_BINARY_DIR}")

# Choose Debug mode by default
if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE "Debug" CACHE STRING "" FORCE)
  message("-- No build mode chosen - using Debug by default")
endif(NOT CMAKE_BUILD_TYPE)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Werror -pthread")

#include_directories()
INCLUDE_DIRECTORIES("${CMAKE_SOURCE_DIR}/src")

# Module Directories
add_subdirectory(src)
# Test Directories


# build main executable
add_executable(main src/main.cpp)
target_link_libraries(main ${MODULES})

# Testing framework
if(ENABLE_TEST)
  enable_testing()
  add_subdirectory(tests)
endif()