#include "IHaversideAlgorithmStrategy.h"

class HaversideKilometerAlgorithm : public IHaversideAlgorithmStrategy
{
public:

    double CalculateDistance(Landmark aCurrentLandmark, Landmark aMappedLandmark);

private:

    const double EARTH_RADIUS_KM = 6371.0;  
};