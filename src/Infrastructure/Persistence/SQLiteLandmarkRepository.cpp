#include <filesystem>
#include <iostream>
#include <string>
#include <format>

#include "DomainConstants.h"
#include "SQLiteLandmarkRepository.h"
#include "Mappers/LandmarkFilterTypeMapper.h"
#include "Mappers/LandmarkFilterContinentMapper.h"

static std::string SQLiteColumnToString(sqlite3_stmt* aStatement, int aColumn)
{
    const auto* Value = reinterpret_cast<const char*>(sqlite3_column_text(aStatement, aColumn));
    return Value ? std::string(Value) : std::string();
}

std::vector<Landmark> SQLiteLandmarkRepository::GetFilteredLandmarks()
{
    std::string DBQuery = DB_STRUCTURE; 

    sqlite3* RawDB = nullptr;
    if (sqlite3_open("DB/landmark.db", &RawDB) != SQLITE_OK) 
    {
        std::string ErrorMessage = RawDB ? sqlite3_errmsg(RawDB) : "Unknown error";
        if (RawDB) sqlite3_close(RawDB);
        throw std::runtime_error(std::format("Infrastructure Error: Cannot open database: {}", ErrorMessage));
    }

    std::unique_ptr<sqlite3, decltype(&sqlite3_close)> DB(RawDB, sqlite3_close);

    sqlite3_stmt* RawStatement = nullptr;
    if (sqlite3_prepare_v2(DB.get(), DBQuery.c_str(), -1, &RawStatement, nullptr) != SQLITE_OK) 
    {
        throw std::runtime_error(std::format("Infrastructure Error: SQL Query preparation failed: {}", sqlite3_errmsg(DB.get())));
    }

    std::unique_ptr<sqlite3_stmt, decltype(&sqlite3_finalize)> Statement(RawStatement, sqlite3_finalize);

    std::vector<Landmark> ReturnValue;

    while (sqlite3_step(Statement.get()) == SQLITE_ROW) 
    {
        std::string Name         = SQLiteColumnToString(Statement.get(), 0);
        double Latitude          = sqlite3_column_double(Statement.get(), 1);
        double Longitude         = sqlite3_column_double(Statement.get(), 2);
        
        std::string TypeStr      = SQLiteColumnToString(Statement.get(), 3);
        LandmarkType Type        = LandmarkFilterTypeMapper::MapStringToType(TypeStr);

        std::string ContinentStr = SQLiteColumnToString(Statement.get(), 4);
        LandmarkContinent Continent = LandmarkFilterContinentMapper::MapStringToContinent(ContinentStr);
        
        std::string ImageUrl     = SQLiteColumnToString(Statement.get(), 5);
        std::string GMapsLink    = SQLiteColumnToString(Statement.get(), 6);

        ReturnValue.emplace_back(std::move(Name), Latitude, Longitude, Type, Continent, std::move(ImageUrl), std::move(GMapsLink));
    }

    return ReturnValue;
}