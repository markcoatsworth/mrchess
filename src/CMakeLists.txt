cmake_minimum_required(VERSION 3.16)

# Set the project name
project(mrchess)

# Specify the C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_CXX_STANDARD_REQUIRED True)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++17")

# We need to explicitly add thread support
set(THREADS_PREFER_PTHREAD_FLAG ON)
find_package(Threads REQUIRED)

# Add the executable
add_executable(mrchess mrchess.cpp)

# Add libraries
add_library(Board Board.cpp)
add_library(MinimaxNode MinimaxNode.cpp)
add_library(MoveEngine MoveEngine.cpp)
add_library(Piece Piece.cpp)

# Link libraries to executable
target_link_libraries(mrchess PRIVATE Threads::Threads)
target_link_libraries(Board Piece)
target_link_libraries(MoveEngine MinimaxNode)
target_link_libraries(mrchess LINK_PUBLIC Board MoveEngine)
