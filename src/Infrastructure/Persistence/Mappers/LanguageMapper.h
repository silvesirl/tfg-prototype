#pragma once
#include <string>
#include <string_view>
#include "Language.h"

namespace LanguageMapper
{
    /**
    * @brief Gives the string language in place of the corresponding enum.
    * @param aLanguageEnum enum to convert to string.
    * @return The corresponding enum.
    */
    Language MapStringToLanguage(std::string aLanguageString);

    /**
    * @brief Gives the Language enum in place of the corresponding string.
    * @param aLanguageString string to convert to enum.
    * @return The corresponding string.
    */
    std::string_view MapLanguageToString(Language aLanguageEnum);
}