# CMakeLists.txt
cmake_minimum_required(VERSION 3.13)
project(mytest LANGUAGES CXX)
 
find_package(STRUMPACK REQUIRED)
find_package(SCALAPACK REQUIRED)
 
add_executable(sp sp.cpp)
add_executable(sp_mpi sp_mpi.cpp)

target_link_libraries(sp PRIVATE STRUMPACK::strumpack)
target_link_libraries(sp_mpi PRIVATE STRUMPACK::strumpack)