#pragma once
#include <string>
#include <iostream>

#include "Landmark.h"
#include "DistanceMetric.h"
#include "IHaversineAlgorithmStrategy.h"
#include "HaversineKilometerAlgorithm.h"
#include "HaversineMilesAlgorithm.h"
#include "HaversineFeetAlgorithm.h"

class DistanceMetricManager
{
public:
    /**
    * @brief Returns the distance metric chosen.
    * @return CurrentDistanceMetric.
    */
    DistanceMetric GetDistanceMetric();

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
    double CalculateDistance(Landmark aLocation1, Landmark aLocation2);
private:
    /**
    * @brief sets the desired strategy to follow with the haversine algorithm.
    * @param aStrategy strategy to follow when calculating distances.
    */
    void SetHaversineStrategy(std::unique_ptr<IHaversineAlgorithmStrategy> aStrategy);

    DistanceMetric CurrentDistanceMetric = DistanceMetric::KILOMETERS;                                                  //!<Current distance metric chosen.
    std::unique_ptr<IHaversineAlgorithmStrategy> HaversineStrategy = std::make_unique<HaversineKilometerAlgorithm>();   //!<haversinestrategy chosen.
};