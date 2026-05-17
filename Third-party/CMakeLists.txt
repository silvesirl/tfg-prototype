add_library(pugixml STATIC 
    pugixml/pugixml.cpp
    pugixml/pugixml.hpp
    pugixml/pugiconfig.hpp
)

target_include_directories(pugixml PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/pugixml")

add_library(sqlite_lib STATIC sqlite3.c)

target_include_directories(sqlite_lib PUBLIC .)