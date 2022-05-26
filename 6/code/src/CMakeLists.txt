# source CMakeLists.txt
cmake_minimum_required(VERSION 3.2)
project(PROJECT)

set(CMAKE_CXX_STANDARD 17)

find_package(OpenCV REQUIRED)
include_directories(${OpenCV_INCLUDE_DIR})

add_executable(restoration main.cpp Filter.cpp Equalization.cpp)
target_link_libraries(restoration  ${OpenCV_LIBS} -lstdc++fs)

