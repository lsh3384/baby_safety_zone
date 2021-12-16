cmake_minimum_required(VERSION 3.1.0 FATAL_ERROR)
project(baby_safety_zone)

include(${CMAKE_CURRENT_SOURCE_DIR}/../socket.io-client-cpp/CMakeLists.txt)

find_package(OpenCV REQUIRED)

add_executable(baby_safety_zone main.cpp base64.cpp)
set_property(TARGET baby_safety_zone PROPERTY CXX_STANDARD 11)
set_property(TARGET baby_safety_zone PROPERTY CXX_STANDARD_REQUIRED ON)
target_link_libraries(baby_safety_zone sioclient)
target_link_libraries(baby_safety_zone pthread)
target_link_libraries(baby_safety_zone ${OpenCV_LIBS})
message(STATUS ${Boost_INCLUDE_DIRS} )
#target_include_directories(sio_console_demo PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/../src" ${Boost_INCLUDE_DIRS} )
