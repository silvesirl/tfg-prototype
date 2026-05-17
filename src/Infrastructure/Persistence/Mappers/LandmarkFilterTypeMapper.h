#include <string_view>
#include "Landmark.h"

namespace LandmarkFilterTypeMapper
{
    inline constexpr std::string_view DB_TYPE_MONUMENT = "monument";
    inline constexpr std::string_view DB_TYPE_NATURAL = "natural";

    /**
    * @brief Gives the string Type in place of the corresponding enum.
    * @param aTypeString enum to convert to string.
    * @return The corresponding enum.
    */
    static LandmarkType MapStringToType(std::string aTypeString)
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
}