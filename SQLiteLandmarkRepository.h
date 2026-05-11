#pragma once
#include <vector>
#include "ILandmarkDBRepository.h" 
#include "Utils/Landmark.h"

extern "C"
{
    #include "sqlite3.h"
}

class SQLiteLandmarkRepository : public ILandmarkDBRepository
{
public:
    SQLiteLandmarkRepository();
    std::vector<Landmark> GetAllLandmarks() override;

    const char* DB_STRUCTURE = "SELECT name, latitude, longitude, type, continent, imageurl, mapsurl FROM landmarks";
};