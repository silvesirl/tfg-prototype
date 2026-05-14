#include <cmath>
#include <algorithm>

#include "LandmarkService.h"

LandmarkService::LandmarkService(ILandmarkDBRepository& repo) : Repository(repo)
{
    SetHaversideStrategy(std::make_unique<HaversideKilometerAlgorithm>());
}

double LandmarkService::ToRadians(double aDegree)
{
    return aDegree * (3.14159265358979323846 / 180.0);
}

void LandmarkService::SetHaversideStrategy(std::unique_ptr<IHaversideAlgorithmStrategy> aStrategy)
{
    this->HaversideStrategy = std::move(aStrategy);
}

double LandmarkService::CalculateHaversine(Landmark aLocation1, Landmark aLocation2)
{
    return this->HaversideStrategy->CalculateDistance(aLocation1, aLocation2);
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