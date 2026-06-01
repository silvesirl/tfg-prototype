#pragma once

#include <memory>
#include "Landmark.h"
#include "DistanceMetric.h"
#include "IHaversineAlgorithmStrategy.h"

class DistanceMetricManager
{
public:
    DistanceMetricManager();

    /**
    * @brief Returns the distance metric chosen.
    * @return CurrentDistanceMetric.
    */
    DistanceMetric GetDistanceMetric() const noexcept;

    /**
    * @brief Sets the distance metric of the manager.
    * @param aDistanceMetric selected metric.
    */
    void SetDistanceMetric(DistanceMetric aDistanceMetric);

    /**
    * @brief Calculate the distance between two given landmarks.
    * @param aLocation1 point of origin.
    * @param aLocation2 point of destiny.
    * @return Calculated distance between the points.
    */
    double CalculateDistance(const Landmark& aLocation1, const Landmark& aLocation2);
private:
    /**
    * @brief sets the desired strategy to follow with the haversine algorithm.
    * @param aStrategy strategy to follow when calculating distances.
    */
    void SetHaversineStrategy(std::unique_ptr<IHaversineAlgorithmStrategy> aStrategy) noexcept;

    DistanceMetric CurrentDistanceMetric;                                   //!<Current distance metric chosen.
    std::unique_ptr<IHaversineAlgorithmStrategy> HaversineStrategy;         //!<haversinestrategy chosen.
};