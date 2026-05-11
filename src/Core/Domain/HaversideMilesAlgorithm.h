#include "IHaversideAlgorithmStrategy.h"

class HaversideMilesAlgorithm : public IHaversideAlgorithmStrategy
{
public:

    double CalculateDistance(Landmark aCurrentLandmark, Landmark aMappedLandmark);

private:

    const double EARTH_RADIUS_MILES = 3958.8;
};