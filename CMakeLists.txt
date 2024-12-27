cmake_minimum_required(VERSION 3.28)
project(operating_system)

set(CMAKE_CXX_STANDARD 14)

add_executable(operating_system
        Code/src/main.cpp
        Code/src/process.cpp
        Code/src/memory.cpp
        Code/src/virtual_memory.cpp
        Code/src/file.cpp
        Code/include/file.h
        Code/include/virtual_memory.h
        Code/include/memory.h
        Code/include/process.h
        Code/src/function.cpp)
