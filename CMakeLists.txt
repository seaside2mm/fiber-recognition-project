cmake_minimum_required(VERSION 3.9)
project(fiber_project)

set(CMAKE_CXX_STANDARD 11)
find_package(OpenCV REQUIRED)
include_directories(${OpenCV_INCLUDE_DIRS})

add_executable(fiber_project main.cpp include/common_include.h include/frame.h include/control.h src/control.cpp src/frame.cpp include/video.h src/video.cpp include/scene.h src/scene.cpp include/config.h src/config.cpp test_example/core_test.cpp test_example/class_test.cpp)

target_link_libraries(fiber_project ${OpenCV_LIBS})
