add_library(sqlite_lib STATIC sqlite3.c)

target_include_directories(sqlite_lib PUBLIC .)