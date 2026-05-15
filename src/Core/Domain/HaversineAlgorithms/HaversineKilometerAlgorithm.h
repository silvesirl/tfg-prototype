#pragma once
#include "IHaversineAlgorithmStrategy.h"

class HaversineKilometerAlgorithm : public IHaversineAlgorithmStrategy
{
public:

    /**
    * @copydoc IHaversineAlgorithmStrategy::CalculateDistance
    */
    double CalculateDistance(const Landmark aCurrentLandmark, const Landmark aMappedLandmark) const override;

private:

    const double EARTH_RADIUS_KM = 6371.0;                          //!< Earth radius for kilometers.
};