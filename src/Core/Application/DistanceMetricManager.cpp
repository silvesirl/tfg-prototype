#include <iostream>
#include <stdexcept>
#include "DistanceMetricManager.h"
#include "HaversineKilometerAlgorithm.h"
#include "HaversineMilesAlgorithm.h"
#include "HaversineFeetAlgorithm.h"

DistanceMetricManager::DistanceMetricManager() : CurrentDistanceMetric(DistanceMetric::KILOMETERS), HaversineStrategy(std::make_unique<HaversineKilometerAlgorithm>())
{

}

DistanceMetric DistanceMetricManager::GetDistanceMetric() const noexcept
{
    return CurrentDistanceMetric;
}

void DistanceMetricManager::SetDistanceMetric(DistanceMetric aDistanceMetric)
{
    std::unique_ptr<IHaversineAlgorithmStrategy> CurrentStrategy = nullptr;

    switch (aDistanceMetric)
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
    
    CurrentDistanceMetric = aDistanceMetric;
    this->HaversineStrategy = std::move(CurrentStrategy);
}

double DistanceMetricManager::CalculateDistance(const Landmark& aLocation1, const Landmark& aLocation2)
{
    return this->HaversineStrategy->CalculateDistance(aLocation1, aLocation2);
}

void DistanceMetricManager::SetHaversineStrategy(std::unique_ptr<IHaversineAlgorithmStrategy> aStrategy) noexcept
{
    this->HaversineStrategy = std::move(aStrategy);
}