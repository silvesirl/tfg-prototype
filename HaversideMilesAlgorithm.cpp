#include <numbers>
#include <cmath>

#include "IHaversideAlgorithmStrategy.h"
#include "HaversideMilesAlgorithm.h"

double HaversideMilesAlgorithm::CalculateDistance(Landmark aCurrentLandmark, Landmark aMappedLandmark)
{
    auto ToRadians = [this](double degree) -> double
    { 
        return degree * (std::numbers::pi / 180.0); 
    };

    double DLat = ToRadians(aMappedLandmark.Lat - aCurrentLandmark.Lat);
    double DLon = ToRadians(aMappedLandmark.Lon - aCurrentLandmark.Lon);

    double ChordLengthSq = std::sin(DLat / 2.0) * std::sin(DLat / 2.0) +
                        std::cos(ToRadians(aCurrentLandmark.Lat)) * std::cos(ToRadians(aMappedLandmark.Lat)) *
                        std::sin(DLon / 2.0) * std::sin(DLon / 2.0);

    double AngularDistanceRadians = 2.0 * std::atan2(std::sqrt(ChordLengthSq), std::sqrt(1.0 - ChordLengthSq));

    double Distance = EARTH_RADIUS_MILES * AngularDistanceRadians;

    return Distance;
}