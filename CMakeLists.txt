cmake_minimum_required(VERSION 3.10)
project(netfin)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(netfin
 src/main.cpp
 src/network/interface_finder.cpp
 src/network/host_prober/host_prober.cpp
 src/network/host_prober/host_prober_apple.cpp
 src/network/host_prober/host_prober_windows.cpp
 src/network/host_prober/host_prober_linux.cpp
 src/utils/utils.cpp
 src/cli/arg_parser.cpp
 
 )

 target_include_directories(netfin PRIVATE include)