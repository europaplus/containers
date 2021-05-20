cmake_minimum_required(VERSION 3.19)
project(test)

set(CMAKE_CXX_STANDARD 98)

add_executable(test
        testing.cpp
        list.cpp
        map.cpp
        queue.cpp
        stack.cpp
        test.hpp
        vector.cpp
        )