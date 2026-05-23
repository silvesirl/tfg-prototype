#pragma once

#include <string>
#include <vector>
#include "Landmark.h"

class ILandmarkDBRepository
{
public:
    virtual ~ILandmarkDBRepository() = default;

    /**
    * @brief gets all landmarks from database repository taking into account the filters.
    * @param aLandmarkContinent Continent to filter by.
    * @param aLandmarkType Type to filter by. 
    * @return a vector with all the filtered landmarks.
    */
    virtual std::vector<Landmark> GetFilteredLandmarks(std::string_view aLandmarkContinent, std::string_view aLandmarkType) = 0;
};