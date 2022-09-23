cmake_minimum_required(VERSION 3.21)
project(Cache)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_FLAGS "-O2")

add_executable(Cache main.cpp lfu_cache.cpp ideal_cache.cpp auxiliary.cpp)