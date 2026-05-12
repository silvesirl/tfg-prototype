#pragma once

#include <vector>
#include <memory>
#include <string>
#include "Landmark.h"
#include "ILandmarkDBRepository.h"

class LandmarkService
{
public:
    LandmarkService(ILandmarkDBRepository& repo);

    double CalculateHaversine(double aLatLocation1, double aLonLocation1, double aLatLocation2, double aLonLocation2);

    std::vector<Landmark> GetProcessedLandmarks(double aLat, double aLon);

    void SetFilteredContinent(std::string aFilteredContinent);

    void SetFilteredCategory(std::string aFilteredCategory);

private:
    ILandmarkDBRepository& Repository;
    double ToRadians(double aDegree);
    LandmarkType TransformStringToType(std::string aLandmarkType);

    std::string FilteredContinent = "-";
    std::string FilteredCategory = "-";
    
    const double EARTH_RADIUS_KM = 6371.0;
};