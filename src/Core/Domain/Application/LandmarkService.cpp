#include <cmath>
#include <algorithm>

#include "LandmarkService.h"

LandmarkService::LandmarkService(ILandmarkDBRepository& repo) : repository(repo) {}

double LandmarkService::ToRadians(double degree) {
    return degree * (3.14159265358979323846 / 180.0);
}

double LandmarkService::CalculateHaversine(double lat1, double lon1, double lat2, double lon2) {
    double dLat = ToRadians(lat2 - lat1);
    double dLon = ToRadians(lon2 - lon1);

    double a = std::sin(dLat / 2.0) * std::sin(dLat / 2.0) +
               std::cos(ToRadians(lat1)) * std::cos(ToRadians(lat2)) *
               std::sin(dLon / 2.0) * std::sin(dLon / 2.0);

    double c = 2.0 * std::atan2(std::sqrt(a), std::sqrt(1.0 - a));
    return EARTH_RADIUS_KM * c;
}

std::vector<Landmark> LandmarkService::GetProcessedLandmarks(
    double userLat, 
    double userLon, 
    const std::string& continentFilter) 
{
    auto allLandmarks = repository.GetAllLandmarks();
    std::vector<Landmark> filteredResults;

    for (auto& CurrentLandmark : allLandmarks)
    {    
        if (continentFilter != "-" && CurrentLandmark.Continent != continentFilter)
        {
            continue;
        }
        
        filteredResults.push_back(CurrentLandmark);
    }

    return filteredResults;
}