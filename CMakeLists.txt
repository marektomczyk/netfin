cmake_minimum_required(VERSION 3.10)
project(netfin)

set(CMAKE_CXX_STANDARD 17)

add_executable(netfin
 src/main.cpp
 src/network/interface_finder.cpp
 src/utils/utils.cpp
 )

 target_include_directories(netfin PRIVATE include)