#include "LandmarkService.h"

LandmarkService::LandmarkService(ILandmarkDBRepository& aRepo, std::shared_ptr<DistanceMetricManager> aMetricManager)
    : Repository(aRepo), MetricManager(aMetricManager)
{
}

double LandmarkService::CalculateDistance(const Landmark& aLocation1, const Landmark& aLocation2)
{
    return this->MetricManager->CalculateDistance(aLocation1, aLocation2);
}

std::vector<Landmark> LandmarkService::GetProcessedLandmarks() 
{
    return Repository.GetFilteredLandmarks(FilteredContinent, FilteredType);
}

void LandmarkService::SetFilteredType(std::string_view aFilteredType)
{
    FilteredType = aFilteredType;
}

void LandmarkService::SetFilteredContinent(std::string_view aFilteredContinent)
{
    FilteredContinent = aFilteredContinent;
}

void LandmarkService::SetMetricDistance(DistanceMetric aDistanceMetric)
{
    this->MetricManager->SetDistanceMetric(aDistanceMetric);
}