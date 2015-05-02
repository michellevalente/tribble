cmake_minimum_required(VERSION 2.8)
project(Tribble)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

include_directories(src)

add_subdirectory(samples)
add_subdirectory(src)

add_executable(dummy dummy.cpp)
target_link_libraries(dummy tribbleLib)