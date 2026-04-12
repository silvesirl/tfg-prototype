#pragma once
#include <string>

enum class LandmarkType
{
    NONE,
    MONUMENT,
    NATURALWONDER
};

struct Landmark
{
    std::string Name;
    double Lat;
    double Lon;
    LandmarkType Type;
    std::string Continent;
};