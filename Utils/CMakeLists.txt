add_library(utils_lib STATIC
    Logger.cpp
    Logger.h
)

target_include_directories(utils_lib PUBLIC .)