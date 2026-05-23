#pragma once

#include <vector>
#include "Landmark.h"
#include "ILandmarkDBRepository.h" 

extern "C"
{
    #include "sqlite3.h"
}

class SQLiteLandmarkRepository : public ILandmarkDBRepository
{
public:
    SQLiteLandmarkRepository() = default;

    /**
    * @copydoc ILandmarkDBRepository::GetFilteredLandmarks
    */
    virtual std::vector<Landmark> GetFilteredLandmarks() override;

private:
    constexpr static const char* DB_STRUCTURE =     "SELECT name, latitude, longitude, type, continent, imageurl, mapsurl FROM landmarks WHERE 1=1";
    constexpr static const char* DB_PATH =          "DB/landmark.db";
};