cmake_minimum_required(VERSION 2.8)

# Set the project name
project(mrchess)

# Specify the C++ standard
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_CXX_STANDARD_REQUIRED True)
set(CMAKE_CXX_FLAGS "-std=c++2a")

# Add the executable
add_executable(mrchess mrchess.cpp)

# Add libraries
add_library(Board Board.cpp)
add_library(Piece Piece.cpp)

# Link libraries to executable
target_link_libraries(mrchess LINK_PUBLIC Board Piece)
