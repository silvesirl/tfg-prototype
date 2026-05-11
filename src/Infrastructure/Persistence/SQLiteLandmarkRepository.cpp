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
        std::cerr << "Error: No se pudo abrir la base de datos: " 
                  << sqlite3_errmsg(DB) << std::endl;
        return {};
    }
    
    if (sqlite3_prepare_v2(DB, DB_STRUCTURE, -1, &Statement, nullptr) != SQLITE_OK) 
    {
        std::cerr << "Error en la consulta SQL: " << sqlite3_errmsg(DB) << std::endl;
        sqlite3_close(DB);
        return {};
    }

    while (sqlite3_step(Statement) == SQLITE_ROW) 
    {
        std::string name = reinterpret_cast<const char*>(sqlite3_column_text(Statement, 0));
        double lat      = sqlite3_column_double(Statement, 1);
        double lon      = sqlite3_column_double(Statement, 2);
        
        std::string typeStr = reinterpret_cast<const char*>(sqlite3_column_text(Statement, 3));
        LandmarkType type = (typeStr == "monument") ? LandmarkType::MONUMENT : LandmarkType::NATURALWONDER;

        std::string cont = reinterpret_cast<const char*>(sqlite3_column_text(Statement, 4));
        std::string img  = reinterpret_cast<const char*>(sqlite3_column_text(Statement, 5));
        std::string maps = reinterpret_cast<const char*>(sqlite3_column_text(Statement, 6));

        ReturnValue.emplace_back(name, lat, lon, type, cont, img, maps);
    }

    sqlite3_finalize(Statement);
    sqlite3_close(DB);

    return ReturnValue;
}