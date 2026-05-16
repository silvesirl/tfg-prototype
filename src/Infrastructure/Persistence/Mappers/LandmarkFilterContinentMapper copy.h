#include <string_view>
#include <unordered_map>
#include "Landmark.h"

namespace LandmarkFilterContinentMapper
{
    static const std::unordered_map<std::string_view, LandmarkContinent> ContinentMap =
    {
        {"Europe",  LandmarkContinent::EUROPE},
        {"Africa",  LandmarkContinent::AFRICA},
        {"Asia",    LandmarkContinent::ASIA},
        {"America", LandmarkContinent::AMERICA},
        {"Oceania", LandmarkContinent::OCEANIA}
    };

    static LandmarkContinent MapToDomain(std::string_view aContinentString)
    {
        auto Result = ContinentMap.find(aContinentString);
        
        if (Result != ContinentMap.end())
        {
            return Result->second;
        }

        return LandmarkContinent::NONE;
    }
}