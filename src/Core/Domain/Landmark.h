#pragma once
#include <string>

enum class LandmarkType
{
    NONE,
    MONUMENT,
    NATURALWONDER
};

enum class LandmarkContinent
{
    NONE,
    EUROPE,
    AFRICA,
    ASIA,
    AMERICA,
    OCEANIA
};

struct Landmark
{
    std::string Name;
    double Lat;
    double Lon;
    LandmarkType Type;
    LandmarkContinent Continent;
    std::string Image;
    std::string MapsLink;
};