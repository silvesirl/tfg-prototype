cmake_minimum_required(VERSION 3.15)
project(GeospatialAPI_Prototype LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)

add_executable(geospatial_api main.cpp)

if(WIN32)
    target_link_libraries(geospatial_api PRIVATE ws2_32)
endif()