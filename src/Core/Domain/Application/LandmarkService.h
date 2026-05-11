#pragma once

#include <vector>
#include <memory>
#include <string>
#include "Landmark.h"
#include "ILandmarkDBRepository.h"

class LandmarkService {
public:
    LandmarkService(ILandmarkDBRepository& repo);

    double CalculateHaversine(double lat1, double lon1, double lat2, double lon2);

    std::vector<Landmark> GetProcessedLandmarks(
        double userLat, 
        double userLon, 
        const std::string& continentFilter
    );

private:
    ILandmarkDBRepository& repository;
    double ToRadians(double degree);
    
    const double EARTH_RADIUS_KM = 6371.0;
};