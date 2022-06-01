# source CMakeLists.txt
cmake_minimum_required(VERSION 3.2)
project(PROJECT)

set(CMAKE_CXX_STANDARD 17)

find_package(OpenCV REQUIRED)
include_directories(${OpenCV_INCLUDE_DIR})

add_executable(camera_calibration main.cpp)
target_link_libraries(camera_calibration  ${OpenCV_LIBS} -lstdc++fs)


