cmake_minimum_required(VERSION 3.15)
project(Backtester)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_compile_options(-Wall -Wextra -pedantic -g)

include_directories(
    src
    src/core
    src/data
    src/indicators
    src/statistics
    src/trade
)

file(GLOB_RECURSE SOURCES "src/*.cpp")

add_executable(backtester ${SOURCES})