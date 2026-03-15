#pragma once
#include <string>

enum class LandmarkType
{
    MONUMENT,
    NATURALWONDER
};

struct Landmark
{
    std::string Name;
    double Lat;
    double Lon;
    LandmarkType Type;
}; 