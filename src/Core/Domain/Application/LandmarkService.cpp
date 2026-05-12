#include <cmath>
#include <algorithm>

#include "LandmarkService.h"

LandmarkService::LandmarkService(ILandmarkDBRepository& repo) : Repository(repo)
{

}

double LandmarkService::ToRadians(double aDegree)
{
    return aDegree * (3.14159265358979323846 / 180.0);
}

double LandmarkService::CalculateHaversine(double aLatLocation1, double aLonLocation1, double aLatLocation2, double aLonLocation2)
{
    double dLat = ToRadians(aLatLocation2 - aLatLocation1);
    double dLon = ToRadians(aLonLocation2 - aLonLocation1);

    double a = std::sin(dLat / 2.0) * std::sin(dLat / 2.0) +
               std::cos(ToRadians(aLatLocation1)) * std::cos(ToRadians(aLatLocation2)) *
               std::sin(dLon / 2.0) * std::sin(dLon / 2.0);

    double c = 2.0 * std::atan2(std::sqrt(a), std::sqrt(1.0 - a));
    return EARTH_RADIUS_KM * c;

    // usar strategia en un futuro
}

LandmarkType LandmarkService::TransformStringToType(std::string aLandmarkType)
{
    if(aLandmarkType == "Monumento")
    {
        return LandmarkType::MONUMENT;
    }
    else if(aLandmarkType == "Natural")
    {
        return LandmarkType::NATURALWONDER;
    }
    else
    {
        return LandmarkType::NONE;
    }
}

std::vector<Landmark> LandmarkService::GetProcessedLandmarks(double aLat, double aLon) 
{
    auto AllLandmarks = Repository.GetAllLandmarks();
    std::vector<Landmark> FilteredResults;

    for (auto& CurrentLandmark : AllLandmarks)
    {    
        if (FilteredContinent != "-" && CurrentLandmark.Continent != FilteredContinent)
        {
            continue;
        }

        if (FilteredCategory != "-" && CurrentLandmark.Type != TransformStringToType(FilteredCategory))
        {
            continue;
        }
        
        FilteredResults.push_back(CurrentLandmark);
    }

    return FilteredResults;
}

void LandmarkService::SetFilteredCategory(std::string aFilteredCategory)
{
    FilteredCategory = aFilteredCategory;
}

void LandmarkService::SetFilteredContinent(std::string aFilteredContinent)
{
    FilteredContinent = aFilteredContinent;
}