#main CmakeLists
cmake_minimum_required(VERSION 3.10) 
project(TTProject LANGUAGES CXX)

set(CMAKE_BUILD_TYPE Debug)

find_package(PkgConfig REQUIRED)

#to build use
#mkdir build
#cd build
#cmake ..
#cmake --build .

# Set policy CMP0079 to NEW 
#cmake_policy(SET CMP0079 NEW)
# Add include directory 
include_directories(${CMAKE_SOURCE_DIR}/include)


add_subdirectory(src)