#pragma once

#include <string_view>
#include "Landmark.h"

namespace LandmarkFilterTypeMapper
{
    inline constexpr std::string_view DB_TYPE_MONUMENT = "monument";
    inline constexpr std::string_view DB_TYPE_NATURAL = "natural";
    inline constexpr std::string_view DB_TYPE_NONE     = "-";

    /**
    * @brief Gives the string Type in place of the corresponding enum.
    * @param aTypeString string to convert to enum.
    * @return The corresponding enum.
    */
    inline LandmarkType MapStringToType(std::string_view aTypeString) noexcept
    {
        if(aTypeString == DB_TYPE_MONUMENT)
        {
            return LandmarkType::MONUMENT;
        }
        else if(aTypeString == DB_TYPE_NATURAL)
        {
            return LandmarkType::NATURALWONDER;
        }
        else
        {
            return LandmarkType::NONE;
        }
    }

    /**
    * @brief Gives the Type in place of the corresponding string.
    * @param aType type to convert to string.
    * @return The corresponding enum.
    */
    inline std::string_view MapTypeToString(LandmarkType aType) noexcept
    {
        switch (aType)
        {
            case LandmarkType::MONUMENT:      return DB_TYPE_MONUMENT;
            case LandmarkType::NATURALWONDER: return DB_TYPE_NATURAL;
            case LandmarkType::NONE:          return DB_TYPE_NONE;
            default:                          return DB_TYPE_NONE;
        }
    }
}