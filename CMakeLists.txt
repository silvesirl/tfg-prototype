cmake_minimum_required(VERSION 3.15)
project(GeospatialAPI LANGUAGES CXX C)

set(CMAKE_CXX_STANDARD 20)

add_library(sqlite_lib STATIC Third-party/sqlite3.c)

add_executable(geospatial_api 
    main.cpp 
    src/Core/Application/LandmarkService.cpp
    src/Core/Domain/HaversineAlgorithms/HaversineKilometerAlgorithm.cpp
    src/Core/Domain/HaversineAlgorithms/HaversineMilesAlgorithm.cpp
    src/Core/Domain/HaversineAlgorithms/HaversineFeetAlgorithm.cpp
    src/Infrastructure/Persistence/Api/LandmarkController.cpp
    src/Infrastructure/Persistence/SQLiteLandmarkRepository.cpp
)

add_custom_command(TARGET geospatial_api POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
        "${CMAKE_CURRENT_SOURCE_DIR}/DB"
        "$<TARGET_FILE_DIR:geospatial_api>/DB"
)

target_include_directories(geospatial_api PRIVATE
    Third-party
    src/Core/Domain
    src/Core/Ports
    src/Core/Application
    src/Core/Domain/HaversineAlgorithms
    src/Infrastructure/Persistence
    src/Infrastructure/Persistence/Api
)

target_link_libraries(geospatial_api PRIVATE sqlite_lib)