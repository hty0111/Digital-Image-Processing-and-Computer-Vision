# source CMakeLists.txt
CMAKE_MINIMUM_REQUIRED(VERSION 3.2)

PROJECT(TEST)

SET(CMAKE_CXX_STANDARD 14)

INCLUDE_DIRECTORIES(${PROJECT_SOURCE_DIR}/image)

find_package(OpenCV REQUIRED)

ADD_EXECUTABLE(histogram_equalization equalization.cpp main.cpp)
target_link_libraries(histogram_equalization ${OpenCV_LIBS})


