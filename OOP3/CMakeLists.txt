cmake_minimum_required(VERSION 4.10)

project(ObjectDetection)

set(OpenCV_DIR C:\\Users\\PC\\Desktop\\dont_open\opencv\build)
find_package(OpenCV REQUIRED)

add_executable(${PROJECT_NAME} src/Main.cpp)

target_link_libraries(${PROJECT_NAME} ${OpenCV_LIB})