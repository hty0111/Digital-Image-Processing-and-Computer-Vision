# source CMakeLists.txt
cmake_minimum_required(VERSION 3.2)

project(TEST)

set(CMAKE_CXX_STANDARD 14)

include_directories(${PROJECT_SOURCE_DIR}/image)

find_package(OpenCV REQUIRED)

add_executable(filter main.cpp Noise.cpp Filter.cpp)
target_link_libraries(filter ${OpenCV_LIBS})


