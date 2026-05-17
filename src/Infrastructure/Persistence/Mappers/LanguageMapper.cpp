#include <unordered_map>
#include "Logger.h"
#include "Persistence/Mappers/LanguageMapper.h"

namespace LanguageMapper
{
    static const std::unordered_map<std::string, Language> StringToLanguageMap =
    {
        {"en",  Language::EN},
        {"es",  Language::ES},
        {"ita", Language::ITA},
        {"fr",  Language::FR},
        {"de",  Language::DE},
        {"ch",  Language::CH},
        {"jp",  Language::JP},
        {"kr",  Language::KR}
    };

    Language MapStringToLanguage(std::string aLanguageString)
    {
        auto Result = StringToLanguageMap.find(aLanguageString);
        
        if (Result != StringToLanguageMap.end())
        {
            return Result->second;
        }

        Logger::Log(Logger::WarningLevel::WARNING, "Param " + aLanguageString + " not found in the map, default to english language");
        return Language::EN;
    }

    std::string_view MapLanguageToString(Language aLanguageEnum)
    {
        switch (aLanguageEnum)
        {
            case Language::EN:  return "en";
            case Language::ES:  return "es";
            case Language::ITA: return "ita";
            case Language::FR:  return "fr";
            case Language::DE:  return "de";
            case Language::CH:  return "ch";
            case Language::JP:  return "jp";
            case Language::KR:  return "kr";
            default:
                Logger::Log(Logger::WarningLevel::WARNING, "Param not found in the language switch, default to english language");
                return "en";
        }
    }
}