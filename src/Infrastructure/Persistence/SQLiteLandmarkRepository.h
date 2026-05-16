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
    SQLiteLandmarkRepository();

    /**
    * @copydoc ILandmarkDBRepository::GetFilteredLandmarks
    */
    virtual std::vector<Landmark> GetFilteredLandmarks(std::string aLandmarkContinent, std::string aLandmarkType) override;

private:
    const char* DB_STRUCTURE =  "SELECT name, latitude, longitude, type, continent, imageurl, mapsurl FROM landmarks WHERE 1=1";
    const char* DB_CONTINENT =  " AND continent = ?";
    const char* DB_TYPE =       " AND type = ?";
};