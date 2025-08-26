cmake_minimum_required(VERSION 3.10)
project(netfin)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

file(GLOB_RECURSE NETFIN_SOURCES
  "src/app/*.cpp"
  "src/cli/*.cpp"
  "src/cli/handlers/*.cpp"
  "src/core/**/*.cpp"
  "src/platform/**/*.cpp"
  "src/platform_factory.cpp"
)

add_executable(netfin ${NETFIN_SOURCES})
target_include_directories(netfin PRIVATE src)