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
src/Debug/data.cpp
src/Debug/data.h
src/Debug/datetime.cpp
src/Debug/datetime.h
src/thrsafe/libh.h
src/Debug/logger.cpp
src/Debug/logger.h
src/main.cpp
src/operations.cpp
src/operations.h
src/thrsafe/ReadersWriterLock.h
src/RunLoop.cpp
src/RunLoop.h
src/Runner.hpp
src/server.cpp
src/server.h
src/thrsafe/shd.h
src/socketclient.cpp
src/socketclient.h
src/thrsafe/thr_safe.h
src/thrsafe/tsmap.h
src/WaitQueue.hpp
)
#target_link_libraries(PosixThreadsLab m)