#include "IHaversideAlgorithmStrategy.h"

class HaversideFeetAlgorithm : public IHaversideAlgorithmStrategy
{
public:

    double CalculateDistance(Landmark aCurrentLandmark, Landmark aMappedLandmark);

private:

    const double EARTH_RADIUS_FEET = 20902231.0;
};