# source CMakeLists.txt
CMAKE_MINIMUM_REQUIRED(VERSION 3.2)

PROJECT(TEST)

SET(CMAKE_CXX_STANDARD 14)

INCLUDE_DIRECTORIES(${PROJECT_SOURCE_DIR}/image)

find_package(OpenCV REQUIRED)

ADD_EXECUTABLE(show_image show_image.cpp)
target_link_libraries(show_image ${OpenCV_LIBS})

ADD_EXECUTABLE(play_video play_video.cpp)
target_link_libraries(play_video ${OpenCV_LIBS})

