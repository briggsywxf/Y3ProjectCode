cmake_minimum_required(VERSION 3.24)
project(OpenCVServer)

set(CMAKE_CXX_STANDARD 14)
string(REPLACE "/O2" "/Od" CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE}")

find_package(OpenCV REQUIRED)
include_directories(${OpenCV_INCLUDE_DIRS})

add_executable(OpenCVServer main.cpp UDPCommunicator.cpp UDPCommunicator.hpp BallTracker.cpp BallTracker.hpp)


target_link_libraries(OpenCVServer wsock32 ws2_32)
target_link_libraries(OpenCVServer ${OpenCV_LIBS})
