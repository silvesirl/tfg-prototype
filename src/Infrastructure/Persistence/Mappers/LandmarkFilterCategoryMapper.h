#include <string_view>
#include "Landmark.h"

namespace LandmarkFilterCategoryMapper
{
    inline constexpr std::string_view DB_TYPE_MONUMENT = "Monumento";
    inline constexpr std::string_view DB_TYPE_NATURAL = "Natural";

    static LandmarkType LandmarkFilterCategoryMapper(std::string aCategoryString)
    {
        if(aCategoryString == "Monumento")
        {
            return LandmarkType::MONUMENT;
        }
        else if(aCategoryString == "Natural")
        {
            return LandmarkType::NATURALWONDER;
        }
        else
        {
            return LandmarkType::NONE;
        }
    }
}