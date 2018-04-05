cmake_minimum_required(VERSION 2.8)
project(img_roi)

set(CMAKE_BUILD_TYPE "Release")
set(CMAKE_CXX_FLAGS "-std=c++11")
set(CMAKE_CXX_FLAGS_RELEASE "-O3 -Wall -g")

find_package(catkin REQUIRED COMPONENTS
    roscpp
    sensor_msgs
    cv_bridge
    )

find_package(OpenCV 3 REQUIRED)

include_directories(${catkin_INCLUDE_DIRS})

catkin_package()

add_executable(img_roi src/img_roi_node.cpp)
target_link_libraries(img_roi ${catkin_LIBRARIES}  ${OpenCV_LIBS})