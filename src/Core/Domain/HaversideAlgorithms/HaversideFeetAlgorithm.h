#pragma once
#include "IHaversideAlgorithmStrategy.h"

class HaversideFeetAlgorithm : public IHaversideAlgorithmStrategy
{
public:

    double CalculateDistance(const Landmark aCurrentLandmark, const Landmark aMappedLandmark) const override;

private:

    const double EARTH_RADIUS_FEET = 20902231.0;
};