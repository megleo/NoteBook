cmake_minimum_required(VERSION 3.23)
project(code)

set(CMAKE_CXX_STANDARD 17)

set(CMAKE_CXX_FLAGS ${CMAKE_CXX_FLAGS} "-pthread")

add_executable(code main.cpp)
