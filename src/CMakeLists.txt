cmake_minimum_required(VERSION 2.8)

# Set the project name
project(mrchess)

# Specify the C++ standard
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_CXX_STANDARD_REQUIRED True)
SET(CMAKE_CXX_FLAGS "-std=c++11")

# Add the executable
add_executable(mrchess mrchess.cpp)

# Add the binary tree to the search path for include files
target_include_directories(mrchess PUBLIC
    "${PROJECT_BINARY_DIR}"
    "${PROJECT_SOURCE_DIR}/include"
)
