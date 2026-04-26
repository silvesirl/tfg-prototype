#pragma once

enum class Language
{
    EN,
    ES,
    ITA,
    FR,
    DE,
    CH,
    JP,
    KR
};

std::map<std::string, Language> LanguageMap =
{
    {"en", Language::EN}, 
    {"es", Language::ES},
    {"ita", Language::ITA},
    {"fr", Language::FR},
    {"de", Language::DE},
    {"ch", Language::CH},
    {"jp", Language::JP},
    {"kr", Language::KR}
};

static Language StringToType(const std::string& aLanguage)
{
    auto Iterator = LanguageMap.find(aLanguage);
    
    if (Iterator != LanguageMap.end()) 
    {
        return Iterator->second;
    }
};