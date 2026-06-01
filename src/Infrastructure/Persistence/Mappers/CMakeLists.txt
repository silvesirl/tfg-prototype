add_library(persistence_mappers INTERFACE)
target_include_directories(persistence_mappers INTERFACE .)
target_sources(persistence_mappers INTERFACE 
    LanguageMapper.h
    LandmarkFilterTypeMapper.h
    LandmarkFilterContinentMapper.h
    DistanceMetricMapper.h
)
