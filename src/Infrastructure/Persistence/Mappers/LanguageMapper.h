#pragma once

#include <string>
#include <string_view>
#include "Language.h"

namespace LanguageMapper
{
    inline constexpr std::string_view LANGUAGE_EN  = "en";
    inline constexpr std::string_view LANGUAGE_ES  = "es";
    inline constexpr std::string_view LANGUAGE_ITA = "ita";
    inline constexpr std::string_view LANGUAGE_FR  = "fr";
    inline constexpr std::string_view LANGUAGE_DE  = "de";
    inline constexpr std::string_view LANGUAGE_CH  = "ch";
    inline constexpr std::string_view LANGUAGE_JP  = "jp";
    inline constexpr std::string_view LANGUAGE_KR  = "kr";

    /**
    * @brief Gives the string language in place of the corresponding enum.
    * @param aLanguageEnum enum to convert to string.
    * @return The corresponding enum.
    */
    inline Language MapStringToLanguage(std::string_view aLanguageString) noexcept
    {
        if (aLanguageString == LANGUAGE_EN)     return Language::EN;
        if (aLanguageString == LANGUAGE_ES)     return Language::ES;
        if (aLanguageString == LANGUAGE_ITA)    return Language::ITA;
        if (aLanguageString == LANGUAGE_FR)     return Language::FR;
        if (aLanguageString == LANGUAGE_DE)     return Language::DE;
        if (aLanguageString == LANGUAGE_CH)     return Language::CH;
        if (aLanguageString == LANGUAGE_JP)     return Language::JP;
        if (aLanguageString == LANGUAGE_KR)     return Language::KR;

        Logger::Log(Logger::WarningLevel::WARNING, "Param " + std::string(aLanguageString) + " not found in the map, default to english language");
        return Language::EN;
    }

    /**
    * @brief Gives the Language enum in place of the corresponding string.
    * @param aLanguageString string to convert to enum.
    * @return The corresponding string.
    */
    inline std::string_view MapLanguageToString(Language aLanguageEnum) noexcept
    {
        switch (aLanguageEnum)
        {
            case Language::EN:                  return LANGUAGE_EN;
            case Language::ES:                  return LANGUAGE_ES;
            case Language::ITA:                 return LANGUAGE_ITA;
            case Language::FR:                  return LANGUAGE_FR;
            case Language::DE:                  return LANGUAGE_DE;
            case Language::CH:                  return LANGUAGE_CH;
            case Language::JP:                  return LANGUAGE_JP;
            case Language::KR:                  return LANGUAGE_KR;
            default:
                Logger::Log(Logger::WarningLevel::WARNING, "Param not found in the language switch, default to english language");
                return LANGUAGE_EN;
        }
    }
}