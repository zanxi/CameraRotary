# CMakeList.txt : CMake project for MServer, include source and define
# project specific logic here.
#
cmake_minimum_required (VERSION 3.1)


project ("CameraRotate")

find_package(Threads)

add_subdirectory (Client)
add_subdirectory (CameraRotate_Tcp_Server)