cmake_minimum_required(VERSION 3.1)
project(CameraServer)

set(CMAKE_CXX_STANDARD 17)
include_directories(/usr/include/x86_64-linux-gnu/mpich)
set(CMAKE_C_COMPILER "/usr/bin/mpicc")
set(CMAKE_CXX_COMPILER "/usr/bin/mpic++")

add_executable(CameraServer
src/camera_action.cpp
src/camera_action.h
src/camera_driver.cpp
src/camera_driver.hpp
src/thrsafe/libh.h
src/main.cpp
src/operations.cpp
src/operations.h
src/thrsafe/ReadersWriterLock.h
src/Server.cpp
src/Server.h
src/sessionclient.cpp
src/sessionclient.h
src/thrsafe/shd.h
src/socketobj.cpp
src/socketobj.h
src/workers/taskqueue.cpp
src/workers/taskqueue.h
src/thrsafe/thr_safe.h
src/workers/threadpool.cpp
src/workers/threadpool.h
src/thrsafe/tsmap.h
src/Debug/data.cpp
src/Debug/data.h
src/Debug/datetime.cpp
src/Debug/datetime.h
src/Debug/logger.cpp
src/Debug/logger.h
)
#target_link_libraries(PosixThreadsLab m)