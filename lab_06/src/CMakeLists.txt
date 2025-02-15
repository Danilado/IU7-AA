cmake_minimum_required(VERSION 3.10)

project(hamilton)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)

add_executable(hamilton 
main.cpp
./impl/AntPF.cpp 
./impl/FullPF.cpp 
)

target_include_directories(hamilton PRIVATE ./inc)
