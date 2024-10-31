cmake_minimum_required(VERSION 3.1)
project(Server)

set(CMAKE_CXX_STANDARD 17)
include_directories(/usr/include/x86_64-linux-gnu/mpich)
set(CMAKE_C_COMPILER "/usr/bin/mpicc")
set(CMAKE_CXX_COMPILER "/usr/bin/mpic++")

add_executable(Server
"src/camera_action.cpp"
"src/camera_action.h"
"src/camera_driver.cpp"
"src/camera_driver.hpp"
"src/thrsafe/libh.h"
"src/main.cpp"
"src/operations.cpp"
"src/operations.h"
"src/thrsafe/ReadersWriterLock.h"
"src/Server.cpp"
"src/Server.h"
"src/thrsafe/shd.h"
"src/thrsafe/thr_safe.h"
"src/thrsafe/tsmap.h"
)
#target_link_libraries(PosixThreadsLab m)