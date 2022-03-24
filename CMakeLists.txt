cmake_minimum_required(VERSION 3.10)

project(wings)


set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

file(GLOB ALL "src/*.cpp")
message(${ALL})
add_executable(wings ${ALL})