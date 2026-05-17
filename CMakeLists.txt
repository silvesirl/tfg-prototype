cmake_minimum_required(VERSION 3.15)
project(GeospatialAPI LANGUAGES CXX C)

set(CMAKE_CXX_STANDARD 20)

include_directories(
    "${CMAKE_CURRENT_SOURCE_DIR}/Utils"
    "${CMAKE_CURRENT_SOURCE_DIR}/Third-party"
    "${CMAKE_CURRENT_SOURCE_DIR}/Third-party/pugixml"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/Core/Domain"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/Core/Ports"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/Core/Application"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/Core/Domain/HaversineAlgorithms"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/Infrastructure/Persistence"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/Infrastructure/Persistence/Api"
)

add_subdirectory(Utils)
add_subdirectory(Third-party)
add_subdirectory(src)

add_executable(geospatial_api main.cpp)

target_link_libraries(geospatial_api PRIVATE 
    core_lib 
    infrastructure_lib 
    sqlite_lib
    utils_lib
    pugixml
)

add_custom_command(TARGET geospatial_api POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
        "${CMAKE_CURRENT_SOURCE_DIR}/DB"
        "$<TARGET_FILE_DIR:geospatial_api>/DB"
    COMMAND ${CMAKE_COMMAND} -E copy
        "${CMAKE_CURRENT_SOURCE_DIR}/localization.xml"
        "$<TARGET_FILE_DIR:geospatial_api>/localization.xml"
)