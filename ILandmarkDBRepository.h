#pragma once
#include <string>
#include <vector>
#include "Utils/Landmark.h"

class ILandmarkDBRepository
{
public:
    virtual ~ILandmarkDBRepository() = default;
    virtual std::vector<Landmark> GetAllLandmarks() = 0;

protected:
    const std::string DB_PATH = "DB/landmark.db";
};