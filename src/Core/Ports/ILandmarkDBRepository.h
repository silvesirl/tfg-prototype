#pragma once
#include <string>
#include <vector>
#include "Landmark.h"

class ILandmarkDBRepository
{
public:
    virtual ~ILandmarkDBRepository() = default;

    /**
    * @brief gets all landmarks from databse repository. 
    * @return a vector with all the landmarks.
    */
    virtual std::vector<Landmark> GetAllLandmarks() = 0;

protected:
    const std::string DB_PATH = "DB/landmark.db";
};