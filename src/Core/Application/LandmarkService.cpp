#include "LandmarkService.h"

LandmarkService::LandmarkService(ILandmarkDBRepository& aRepo, std::shared_ptr<DistanceMetricManager> aMetricManager)
    : Repository(aRepo), MetricManager(aMetricManager)
{
}

void LandmarkService::Initialize()
{
    Landmarks = Repository.GetFilteredLandmarks();
}

double LandmarkService::CalculateDistance(const Landmark& aLocation1, const Landmark& aLocation2)
{
    return this->MetricManager->CalculateDistance(aLocation1, aLocation2);
}

std::vector<Landmark> LandmarkService::GetProcessedLandmarks() 
{
    std::vector<Landmark> ReturnLandmarks;
    ReturnLandmarks.reserve(Landmarks.size());

    for (const auto& CurrentLandmark : Landmarks)
    {
        if (FilteredType != LandmarkType::NONE)
        {
            if(CurrentLandmark.Type != FilteredType)
            {
                continue;
            }
        }

        if (FilteredContinent != LandmarkContinent::NONE)
        {
            if(CurrentLandmark.Continent != FilteredContinent)
            {
                continue;
            }
        }

        ReturnLandmarks.push_back(CurrentLandmark);
    }

    return ReturnLandmarks;
}

void LandmarkService::SetFilteredContinent(LandmarkContinent aContinentToFilter)
{
    FilteredContinent = aContinentToFilter;
}

void LandmarkService::SetFilteredType(LandmarkType aTypeToFilter)
{
    FilteredType = aTypeToFilter;
}

void LandmarkService::SetMetricDistance(DistanceMetric aDistanceMetric)
{
    this->MetricManager->SetDistanceMetric(aDistanceMetric);
}