add_subdirectory(Api)

target_sources(infrastructure_lib PRIVATE
    SQLiteLandmarkRepository.cpp
)