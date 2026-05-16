add_subdirectory(Api)
add_subdirectory(Mappers)

target_sources(infrastructure_lib PRIVATE
    SQLiteLandmarkRepository.cpp
)