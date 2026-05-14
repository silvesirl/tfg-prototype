#pragma once

#include <vector>
#include <memory>
#include <string>
#include "Landmark.h"
#include "ILandmarkDBRepository.h"
#include "IHaversideAlgorithmStrategy.h"
#include "HaversideKilometerAlgorithm.h"
#include "HaversideMilesAlgorithm.h"
#include "HaversideFeetAlgorithm.h"

class LandmarkService
{
public:
    LandmarkService(ILandmarkDBRepository& repo);

    double CalculateHaversine(Landmark aLocation1, Landmark aLocation2);

    std::vector<Landmark> GetProcessedLandmarks(double aLat, double aLon);

    void SetFilteredContinent(std::string aFilteredContinent);

    void SetFilteredCategory(std::string aFilteredCategory);

    void SetHaversideStrategy(std::unique_ptr<IHaversideAlgorithmStrategy> aStrategy);

private:
    ILandmarkDBRepository& Repository;
    double ToRadians(double aDegree);
    LandmarkType TransformStringToType(std::string aLandmarkType);
    std::unique_ptr<IHaversideAlgorithmStrategy> HaversideStrategy;

    std::string FilteredContinent = "-";
    std::string FilteredCategory = "-";
    
    const double EARTH_RADIUS_KM = 6371.0;
};