# source CMakeLists.txt
cmake_minimum_required(VERSION 3.2)
project(PROJECT)

set(CMAKE_CXX_STANDARD 14)

find_package(OpenCV REQUIRED)

add_executable(segment main.cpp Segment.cpp)
target_link_libraries(segment ${OpenCV_LIBS})

