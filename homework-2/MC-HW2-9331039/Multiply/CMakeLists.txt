cmake_minimum_required(VERSION 3.10)
project(Multiply)

# added -fopenmp
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -fopenmp")

set(CMAKE_CXX_STANDARD 11)
set(SOURCE_FILES Multiply_1Dimensional.cpp)

add_executable(Multiply Multiply_1Dimensional.cpp)




