#pragma once
#include "IHaversineAlgorithmStrategy.h"

class HaversineKilometerAlgorithm : public IHaversineAlgorithmStrategy
{
protected:
    /**
    * @copydoc IHaversineAlgorithmStrategy::GetEarthRadius
    */
    virtual double GetEarthRadius() const override
    {
        return EARTH_RADIUS_KM;
    };

private:
    const double EARTH_RADIUS_KM = 6371.0;                          //!< Earth radius for kilometers.
};