#pragma once
#include "IHaversideAlgorithmStrategy.h"

class HaversideKilometerAlgorithm : public IHaversideAlgorithmStrategy
{
public:

    /**
    * @copydoc IHaversideAlgorithmStrategy::CalculateDistance
    */
    double CalculateDistance(const Landmark aCurrentLandmark, const Landmark aMappedLandmark) const override;

private:

    const double EARTH_RADIUS_KM = 6371.0;                          //!< Earth radius for kilometers.
};