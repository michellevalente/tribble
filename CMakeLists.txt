cmake_minimum_required(VERSION 2.8)
project(Tribble)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

include_directories(src)

#set(SOURCE_FILES main.cpp)
#add_executable(Main ${SOURCE_FILES})

add_subdirectory(samples)


