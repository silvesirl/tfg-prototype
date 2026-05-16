#pragma once
#include "IHaversineAlgorithmStrategy.h"

class HaversineMilesAlgorithm : public IHaversineAlgorithmStrategy
{
protected:
    /**
    * @copydoc IHaversineAlgorithmStrategy::GetEarthRadius
    */
    virtual double GetEarthRadius() const override
    {
        return EARTH_RADIUS_MILES;
    };

private:
    const double EARTH_RADIUS_MILES = 3958.8;                          //!< Earth radius for miles.
};