#pragma once

#include <string>
#include <vector>
#include "Landmark.h"

class ILandmarkDBRepository
{
public:
    virtual ~ILandmarkDBRepository() = default;

    /**
    * @brief gets all landmarks from database repository.
    * @return a vector with all the landmarks.
    */
    virtual std::vector<Landmark> GetFilteredLandmarks() = 0;
};