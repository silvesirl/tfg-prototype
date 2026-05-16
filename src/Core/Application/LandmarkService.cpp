#include <cmath>
#include <numbers>
#include <algorithm>

#include "LandmarkService.h"

LandmarkService::LandmarkService(ILandmarkDBRepository& Repo) : Repository(Repo)
{
    SetHaversineStrategy(std::make_unique<HaversineKilometerAlgorithm>());
}

void LandmarkService::SetHaversineStrategy(std::unique_ptr<IHaversineAlgorithmStrategy> aStrategy)
{
    this->HaversineStrategy = std::move(aStrategy);
}

double LandmarkService::CalculateHaversine(Landmark aLocation1, Landmark aLocation2)
{
    return this->HaversineStrategy->CalculateDistance(aLocation1, aLocation2);
}

std::vector<Landmark> LandmarkService::GetProcessedLandmarks() 
{
    auto AllLandmarks = Repository.GetAllLandmarks();
    std::vector<Landmark> FilteredResults;

    for (auto& CurrentLandmark : AllLandmarks)
    {    
        if (FilteredContinent != "-" && CurrentLandmark.Continent != FilteredContinent)
        {
            continue;
        }

        if (FilteredCategory != "-" && CurrentLandmark.Type != LandmarkFilterCategoryMapper::LandmarkFilterCategoryMapper(FilteredCategory))
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