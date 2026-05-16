#include <filesystem>
#include <iostream>
#include <string>

#include "DomainConstants.h"
#include "SQLiteLandmarkRepository.h"
#include "Mappers/LandmarkFilterTypeMapper.h"

SQLiteLandmarkRepository::SQLiteLandmarkRepository() 
{
}

std::vector<Landmark> SQLiteLandmarkRepository::GetFilteredLandmarks(std::string aLandmarkContinent, std::string aLandmarkType)
{
    sqlite3* DB = nullptr;
    sqlite3_stmt* Statement = nullptr;
    std::vector<Landmark> ReturnValue;
    std::string DBQuery = DB_STRUCTURE;

    bool AddContinentFilter = false;
    bool AddTypeFilter = false;

    if (aLandmarkContinent != DomainConstants::EMPTY_FILTER)
    {
        DBQuery += DB_CONTINENT;
        AddContinentFilter = true;
    }
    if (aLandmarkType != DomainConstants::EMPTY_FILTER)
    {
        DBQuery += DB_TYPE;
        AddTypeFilter = true;
    }

    if (sqlite3_open("DB/landmark.db", &DB) != SQLITE_OK) 
    {
        std::cerr << "Error: cannot open the database: " 
                  << sqlite3_errmsg(DB) << std::endl;
        return {};
    }

    if (sqlite3_prepare_v2(DB, DBQuery.c_str(), -1, &Statement, nullptr) != SQLITE_OK) 
    {
        std::cerr << "Error SQL query: " << sqlite3_errmsg(DB) << std::endl;
        sqlite3_close(DB);
        return {};
    }

    int Index = 1;

    if(AddContinentFilter)
    {
        sqlite3_bind_text(Statement, Index++, aLandmarkContinent.c_str(), -1, SQLITE_TRANSIENT);
    }
    if(AddTypeFilter)
    {
        sqlite3_bind_text(Statement, Index++, aLandmarkType.c_str(), -1, SQLITE_TRANSIENT);
    }

    while (sqlite3_step(Statement) == SQLITE_ROW) 
    {
        std::string Name = reinterpret_cast<const char*>(sqlite3_column_text(Statement, 0));
        double Latitude      = sqlite3_column_double(Statement, 1);
        double Longitude      = sqlite3_column_double(Statement, 2);
        
        std::string TypeStr = reinterpret_cast<const char*>(sqlite3_column_text(Statement, 3));
        LandmarkType Type = LandmarkFilterTypeMapper::LandmarkFilterTypeMapper(TypeStr);

        std::string Continent = reinterpret_cast<const char*>(sqlite3_column_text(Statement, 4));
        std::string ImageUrl  = reinterpret_cast<const char*>(sqlite3_column_text(Statement, 5));
        std::string GMapsLink = reinterpret_cast<const char*>(sqlite3_column_text(Statement, 6));

        ReturnValue.emplace_back(Name, Latitude, Longitude, Type, Continent, ImageUrl, GMapsLink);
    }

    sqlite3_finalize(Statement);
    sqlite3_close(DB);

    return ReturnValue;
}