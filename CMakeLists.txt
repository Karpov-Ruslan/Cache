cmake_minimum_required(VERSION 3.8)
project(Cache)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_FLAGS "-O2")
set(LFU_FLAGS "-D LFU")
set(IDEAL_CACHE_FLAGS "-D IDEAL")


add_executable(Cache main.cpp)
set_target_properties (Cache PROPERTIES COMPILE_FLAGS "${CMAKE_CXX_FLAGS} ${LFU_FLAGS} ${IDEAL_CACHE_FLAGS}")

add_executable(LFU main.cpp)
set_target_properties (LFU PROPERTIES COMPILE_FLAGS "${CMAKE_CXX_FLAGS} ${LFU_FLAGS}")

add_executable(Ideal main.cpp)
set_target_properties (Ideal PROPERTIES COMPILE_FLAGS "${CMAKE_CXX_FLAGS} ${IDEAL_CACHE_FLAGS}")

add_executable(Test test.cpp)