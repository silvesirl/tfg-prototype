#include <iostream>

#include "DistanceMetricManager.h"

DistanceMetric DistanceMetricManager::GetDistanceMetric()
{
    return CurrentDistanceMetric;
}

void DistanceMetricManager::SetDistanceMetric(DistanceMetric aDistanceMetric)
{
    CurrentDistanceMetric = aDistanceMetric;
    std::unique_ptr<IHaversineAlgorithmStrategy> CurrentStrategy = nullptr;

    switch (CurrentDistanceMetric)
    {
        case DistanceMetric::KILOMETERS:
            CurrentStrategy = std::make_unique<HaversineKilometerAlgorithm>();
            break;
            
        case DistanceMetric::MILES:
            CurrentStrategy = std::make_unique<HaversineMilesAlgorithm>();
            break;
            
        case DistanceMetric::FOOT:
            CurrentStrategy = std::make_unique<HaversineFeetAlgorithm>();
            break;
    }

    this->HaversineStrategy = std::move(CurrentStrategy);
}

double DistanceMetricManager::CalculateDistance(Landmark aLocation1, Landmark aLocation2)
{
    return this->HaversineStrategy->CalculateDistance(aLocation1, aLocation2);
}

void DistanceMetricManager::SetHaversineStrategy(std::unique_ptr<IHaversineAlgorithmStrategy> aStrategy)
{
    this->HaversineStrategy = std::move(aStrategy);
}