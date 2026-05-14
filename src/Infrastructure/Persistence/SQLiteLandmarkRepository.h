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
    * @copydoc ILandmarkDBRepository::GetAllLandmarks
    */
    std::vector<Landmark> GetAllLandmarks() override;

private:
    const char* DB_STRUCTURE = "SELECT name, latitude, longitude, type, continent, imageurl, mapsurl FROM landmarks";
};