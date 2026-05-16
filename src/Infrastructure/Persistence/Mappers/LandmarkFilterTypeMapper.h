#include <string_view>
#include "Landmark.h"

namespace LandmarkFilterTypeMapper
{
    inline constexpr std::string_view DB_TYPE_MONUMENT = "monument";
    inline constexpr std::string_view DB_TYPE_NATURAL = "natural";

    static LandmarkType LandmarkFilterTypeMapper(std::string aTypeString)
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