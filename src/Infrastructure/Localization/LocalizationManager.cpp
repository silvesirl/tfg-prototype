#include <fstream>
#include "pugixml.hpp"

#include "Logger.h"
#include "Persistence/Mappers/LanguageMapper.h"
#include "Localization/LocalizationManager.h"

pugi::xml_document LocalizationManager::Doc; 
bool LocalizationManager::IsInitialized = false;
thread_local Language LocalizationManager::CurrentLanguage = Language::EN;

bool LocalizationManager::Initialize(const std::string_view aFilePath)
{
    pugi::xml_parse_result Result = Doc.load_file(std::string(aFilePath).c_str());

    if (!Result)
    {
        Logger::Log(Logger::WarningLevel::WARNING, "path " + std::string(aFilePath) + " cound not be opened, the translations will not work");
        
        Logger::Log(Logger::WarningLevel::WARNING, "XML Error: " + std::string(Result.description()));
        
        IsInitialized = false;
        return false;
    }

    Logger::Log(Logger::WarningLevel::INFO, "Localizations file parsed succesfully");
    IsInitialized = true;

    return IsInitialized;
}

std::string LocalizationManager::GetTranslation(std::string_view aLabel)
{
    if (!IsInitialized || Doc.empty())
    {
        Logger::Log(Logger::WarningLevel::WARNING, "translations not working, check the path or the xml formatting");
        return std::string(aLabel);
    }

    pugi::xml_node LabelNode = Doc.child("localization").find_child_by_attribute("label", "id", std::string(aLabel).c_str());

    if (LabelNode)
    {
        std::string LanguageString = std::string(LanguageMapper::MapLanguageToString(CurrentLanguage));

        pugi::xml_node TranslationNode = LabelNode.child(LanguageString.c_str());

        if (TranslationNode)
        {
            return TranslationNode.text().as_string(std::string(aLabel).c_str());
        }
        else
        {
            Logger::Log(Logger::WarningLevel::WARNING, "Language node <" + LanguageString + "> not found for label " + std::string(aLabel));
        }
    }
    else
    {
        Logger::Log(Logger::WarningLevel::WARNING, "Label " + std::string(aLabel) + " not found in the xml, check the label or the languages available");
    }

    return std::string(aLabel);
}

void LocalizationManager::SetLanguage(Language aLanguage)
{
    CurrentLanguage = aLanguage;
}