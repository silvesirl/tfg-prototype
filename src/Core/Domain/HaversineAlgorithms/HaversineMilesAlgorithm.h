#pragma once
#include "IHaversineAlgorithmStrategy.h"

class HaversineMilesAlgorithm : public IHaversineAlgorithmStrategy
{
public:

    /**
    * @copydoc IHaversineAlgorithmStrategy::CalculateDistance
    */
    double CalculateDistance(const Landmark aCurrentLandmark, const Landmark aMappedLandmark) const override;

private:

    const double EARTH_RADIUS_MILES = 3958.8;                       //!< Earth radius for miles.
};