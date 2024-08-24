cmake_minimum_required(VERSION 3.28)
project(libmimc)

set(CMAKE_CXX_STANDARD 14)

add_executable(libmimc
        Field.cpp
        main.cpp
        MiMC.cpp)
