#pragma once
#include "Landmark.h"
#include <numbers>

class IHaversineAlgorithmStrategy
{
public:
    virtual ~IHaversineAlgorithmStrategy() = default;

    /**
    * @brief Calculates the distance between two geographic points. 
    * @param aCurrentLandmark Origin point (usually user's location).
    * @param aMappedLandmark Destination point (the POI to measure).
    * @return The calculated distance in the strategy's specific unit.
    */
    virtual double CalculateDistance(const Landmark aCurrentLandmark, const Landmark aMappedLandmark) const
    {
        double DLat = ToRadians(aMappedLandmark.Lat - aCurrentLandmark.Lat);
        double DLon = ToRadians(aMappedLandmark.Lon - aCurrentLandmark.Lon);

        double ChordLengthSq = std::sin(DLat / 2.0) * std::sin(DLat / 2.0) +
                            std::cos(ToRadians(aCurrentLandmark.Lat)) * std::cos(ToRadians(aMappedLandmark.Lat)) *
                            std::sin(DLon / 2.0) * std::sin(DLon / 2.0);

        double AngularDistanceRadians = 2.0 * std::atan2(std::sqrt(ChordLengthSq), std::sqrt(1.0 - ChordLengthSq));

        double Distance = GetEarthRadius() * AngularDistanceRadians;

        return Distance;
    }

protected:
    /**
    * @brief Gets the earth radius constant, this will be diferent if we are calculating distances in kilometers, miles, feet... 
    * @return Earth radius constant according the selected strategy.
    */
    virtual double GetEarthRadius() const = 0;

private:
    /**
    * @brief pass the degrees into radians. 
    * @param aDegree to transform.
    * @return transformed degrees into radians.
    */
    static double ToRadians(double aDegree)
    { 
        return aDegree * (std::numbers::pi / 180.0); 
    };
};