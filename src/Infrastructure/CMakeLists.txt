add_library(infrastructure_lib STATIC "")

target_include_directories(infrastructure_lib PUBLIC 
    ${CMAKE_CURRENT_SOURCE_DIR}
)

add_subdirectory(Localization)
add_subdirectory(Persistence)