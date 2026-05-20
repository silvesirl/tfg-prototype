#include <cmath>
#include <numbers>
#include <algorithm>

#include "LandmarkService.h"
#include "DistanceMetricManager.h"
#include "Mappers/DistanceMetricMapper.h"

LandmarkService::LandmarkService(ILandmarkDBRepository& aRepo, std::shared_ptr<DistanceMetricManager> aMetricManager)
    : Repository(aRepo), MetricManager(aMetricManager)
{
}

double LandmarkService::CalculateDistance(Landmark aLocation1, Landmark aLocation2)
{
    return this->MetricManager->CalculateDistance(aLocation1, aLocation2);
}

std::vector<Landmark> LandmarkService::GetProcessedLandmarks() 
{
    return Repository.GetFilteredLandmarks(FilteredContinent, FilteredType);
}

void LandmarkService::SetFilteredType(std::string aFilteredType)
{
    FilteredType = aFilteredType;
}

void LandmarkService::SetFilteredContinent(std::string aFilteredContinent)
{
    FilteredContinent = aFilteredContinent;
}

void LandmarkService::SetMetricDistance(std::string aDistanceMetricString)
{
    this->MetricManager->SetDistanceMetric(DistanceMetricMapper::MapStringToDistanceMetric(aDistanceMetricString));
}