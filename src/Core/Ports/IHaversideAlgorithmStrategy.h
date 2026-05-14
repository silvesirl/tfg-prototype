#pragma once
#include "Landmark.h"
#include <numbers>

class IHaversideAlgorithmStrategy
{
public:
    virtual ~IHaversideAlgorithmStrategy() = default;

    /**
    * @brief Calculates the distance between two geographic points. 
    * @param aCurrentLandmark Origin point (usually user's location).
    * @param aMappedLandmark Destination point (the POI to measure).
    * @return The calculated distance in the strategy's specific unit.
    */
    virtual double CalculateDistance(const Landmark aCurrentLandmark, const Landmark aMappedLandmark) const = 0;
};