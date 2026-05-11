cmake_minimum_required(VERSION 3.15)
project(GeospatialAPI LANGUAGES CXX C)

set(CMAKE_CXX_STANDARD 20)

add_library(sqlite_lib STATIC sqlite3.c)

add_executable(geospatial_api 
    main.cpp 
    SQLiteLandmarkRepository.cpp
)

add_custom_command(TARGET geospatial_api POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
        "${CMAKE_CURRENT_SOURCE_DIR}/DB"
        "$<TARGET_FILE_DIR:geospatial_api>/DB"
)

target_link_libraries(geospatial_api PRIVATE sqlite_lib)