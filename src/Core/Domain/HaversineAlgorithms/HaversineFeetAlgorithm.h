#pragma once
#include "IHaversineAlgorithmStrategy.h"

class HaversineFeetAlgorithm : public IHaversineAlgorithmStrategy
{
protected:
    /**
    * @copydoc IHaversineAlgorithmStrategy::GetEarthRadius
    */
    virtual double GetEarthRadius() const override
    {
        return EARTH_RADIUS_FEET;
    };

private:
    const double EARTH_RADIUS_FEET = 20902231.0;                    //!< Earth radius for feet.
};