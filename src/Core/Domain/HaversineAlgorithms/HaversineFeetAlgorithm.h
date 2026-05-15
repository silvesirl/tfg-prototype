#pragma once
#include "IHaversineAlgorithmStrategy.h"

class HaversineFeetAlgorithm : public IHaversineAlgorithmStrategy
{
public:

    /**
    * @copydoc IHaversineAlgorithmStrategy::CalculateDistance
    */
    double CalculateDistance(const Landmark aCurrentLandmark, const Landmark aMappedLandmark) const override;

private:

    const double EARTH_RADIUS_FEET = 20902231.0;                    //!< Earth radius for feet.
};