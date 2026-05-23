#pragma once

#include <string_view>
#include "Landmark.h"

namespace LandmarkFilterContinentMapper
{
    inline constexpr std::string_view EUROPE_STRING  = "Europe";
    inline constexpr std::string_view AFRICA_STRING  = "Africa";
    inline constexpr std::string_view ASIA_STRING    = "Asia";
    inline constexpr std::string_view AMERICA_STRING = "America";
    inline constexpr std::string_view OCEANIA_STRING = "Oceania";
    inline constexpr std::string_view NONE_STRING    = "-";

    /**
    * @brief Gives the Continent enum in place of the corresponding string.
    * @param aContinent string to convert to enum.
    * @return The corresponding enum.
    */
    inline LandmarkContinent MapStringToContinent(std::string_view aContinentString)
    {
        if (aContinentString == EUROPE_STRING)  return LandmarkContinent::EUROPE;
        if (aContinentString == AFRICA_STRING)  return LandmarkContinent::AFRICA;
        if (aContinentString == ASIA_STRING)    return LandmarkContinent::ASIA;
        if (aContinentString == AMERICA_STRING) return LandmarkContinent::AMERICA;
        if (aContinentString == OCEANIA_STRING) return LandmarkContinent::OCEANIA;

        return LandmarkContinent::NONE;
    }

    /**
    * @brief Gives the string Continent in place of the corresponding enum.
    * @param aContinentString enum to convert to string.
    * @return The corresponding string.
    */
    inline std::string_view MapContinentToString(LandmarkContinent aContinent)
    {
        switch (aContinent)
        {
            case LandmarkContinent::EUROPE:  return EUROPE_STRING;
            case LandmarkContinent::AFRICA:  return AFRICA_STRING;
            case LandmarkContinent::ASIA:    return ASIA_STRING;
            case LandmarkContinent::AMERICA: return AMERICA_STRING;
            case LandmarkContinent::OCEANIA: return OCEANIA_STRING;
            case LandmarkContinent::NONE:    return NONE_STRING;
            default:                         return NONE_STRING;
        }
    }
}