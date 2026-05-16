#include <filesystem>
#include <iostream>
#include <string>

#include "SQLiteLandmarkRepository.h"

SQLiteLandmarkRepository::SQLiteLandmarkRepository() 
{
}

std::vector<Landmark> SQLiteLandmarkRepository::GetAllLandmarks()
{
    sqlite3* DB = nullptr;
    sqlite3_stmt* Statement = nullptr;
    std::vector<Landmark> ReturnValue;

    if (sqlite3_open("DB/landmark.db", &DB) != SQLITE_OK) 
    {
        std::cerr << "Error: cannot open the database: " 
                  << sqlite3_errmsg(DB) << std::endl;
        return {};
    }
    
    if (sqlite3_prepare_v2(DB, DB_STRUCTURE, -1, &Statement, nullptr) != SQLITE_OK) 
    {
        std::cerr << "Error SQL query: " << sqlite3_errmsg(DB) << std::endl;
        sqlite3_close(DB);
        return {};
    }

    while (sqlite3_step(Statement) == SQLITE_ROW) 
    {
        std::string Name = reinterpret_cast<const char*>(sqlite3_column_text(Statement, 0));
        double Latitude      = sqlite3_column_double(Statement, 1);
        double Longitude      = sqlite3_column_double(Statement, 2);
        
        std::string TypeStr = reinterpret_cast<const char*>(sqlite3_column_text(Statement, 3));
        LandmarkType Type = (TypeStr == "monument") ? LandmarkType::MONUMENT : LandmarkType::NATURALWONDER;

        std::string Continent = reinterpret_cast<const char*>(sqlite3_column_text(Statement, 4));
        std::string ImageUrl  = reinterpret_cast<const char*>(sqlite3_column_text(Statement, 5));
        std::string GMapsLink = reinterpret_cast<const char*>(sqlite3_column_text(Statement, 6));

        ReturnValue.emplace_back(Name, Latitude, Longitude, Type, Continent, ImageUrl, GMapsLink);
    }

    sqlite3_finalize(Statement);
    sqlite3_close(DB);

    return ReturnValue;
}