# source CMakeLists.txt
cmake_minimum_required(VERSION 3.2)
project(PROJECT)

set(CMAKE_CXX_STANDARD 14)

find_package(OpenCV REQUIRED)
include_directories(${OpenCV_INCLUDE_DIR})

add_executable(thinning main.cpp)
target_link_libraries(thinning ${OpenCV_LIBS})

