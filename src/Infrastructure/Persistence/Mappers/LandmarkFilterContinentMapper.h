#include <string_view>
#include <unordered_map>
#include "Landmark.h"

namespace LandmarkFilterContinentMapper
{
    static const std::unordered_map<std::string, LandmarkContinent> ContinentMap =
    {
        {"Europe",  LandmarkContinent::EUROPE},
        {"Africa",  LandmarkContinent::AFRICA},
        {"Asia",    LandmarkContinent::ASIA},
        {"America", LandmarkContinent::AMERICA},
        {"Oceania", LandmarkContinent::OCEANIA}
    };

    /**
    * @brief Gives the string Continent in place of the corresponding enum.
    * @param aContinentString enum to convert to string.
    * @return The corresponding enum.
    */
    static LandmarkContinent MapStringToType(std::string aContinentString)
    {
        auto Result = ContinentMap.find(aContinentString);
        
        if (Result != ContinentMap.end())
        {
            return Result->second;
        }

        return LandmarkContinent::NONE;
    }

    /**
    * @brief Gives the Continent enum in place of the corresponding string.
    * @param aContinent string to convert to enum.
    * @return The corresponding string.
    */
    static std::string_view MapContinentToString(LandmarkContinent aContinent)
    {
        switch (aContinent)
        {
            case LandmarkContinent::EUROPE:     return "Europe";
            case LandmarkContinent::AFRICA:     return "Africa";
            case LandmarkContinent::ASIA:       return "Asia";
            case LandmarkContinent::AMERICA:    return "America";
            case LandmarkContinent::OCEANIA:    return "Oceania";
            case LandmarkContinent::NONE:       return "-";
        }
    }
}