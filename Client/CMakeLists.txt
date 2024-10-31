cmake_minimum_required(VERSION 3.1)
project(Client)

set(CMAKE_CXX_STANDARD 17)
include_directories(/usr/include/x86_64-linux-gnu/mpich)
set(CMAKE_C_COMPILER "/usr/bin/mpicc")
set(CMAKE_CXX_COMPILER "/usr/bin/mpic++")

#add_executable(Client Client.cpp)
add_executable(Client threadpool_posix.cpp)
#target_link_libraries(PosixThreadsLab m)
