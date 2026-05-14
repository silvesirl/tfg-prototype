#pragma once
#include "IHaversideAlgorithmStrategy.h"

class HaversideMilesAlgorithm : public IHaversideAlgorithmStrategy
{
public:

    double CalculateDistance(const Landmark aCurrentLandmark, const Landmark aMappedLandmark) const override;

private:

    const double EARTH_RADIUS_MILES = 3958.8;
};