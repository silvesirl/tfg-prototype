#pragma once
#include "../Landmark.h"
#include <numbers>

class IHaversideAlgorithmStrategy
{
public:
    virtual ~IHaversideAlgorithmStrategy() = default;

    virtual double CalculateDistance(const Landmark& aCurrentLandmark, const Landmark& aMappedLandmark) const = 0;
};