cmake_minimum_required(VERSION 3.15)
project(GeospatialAPI LANGUAGES CXX C)

set(CMAKE_CXX_STANDARD 20)

add_library(sqlite_lib STATIC src/Infrastructure/Persistence/sqlite3.c)

add_executable(geospatial_api 
    main.cpp 
    src/Infrastructure/Persistence/SQLiteLandmarkRepository.cpp
)

add_custom_command(TARGET geospatial_api POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
        "${CMAKE_CURRENT_SOURCE_DIR}/DB"
        "$<TARGET_FILE_DIR:geospatial_api>/DB"
)

target_include_directories(geospatial_api PRIVATE 
    src/Core/Domain
    src/Core/Ports
    src/Core/Application
    src/Infrastructure/Persistence
)

target_link_libraries(geospatial_api PRIVATE sqlite_lib)