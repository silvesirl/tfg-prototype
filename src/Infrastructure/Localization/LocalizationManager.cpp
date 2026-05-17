#include <fstream>
#include "pugixml.hpp"

#include "Logger.h"
#include "Persistence/Mappers/LanguageMapper.h"
#include "Localization/LocalizationManager.h"

pugi::xml_document LocalizationManager::Doc; 
bool LocalizationManager::IsInitialized = false;
Language LocalizationManager::CurrentLanguage = Language::EN;

bool LocalizationManager::Initialize(const std::string& aFilePath)
{
    pugi::xml_parse_result Result = Doc.load_file(aFilePath.c_str());

    if (!Result)
    {
        Logger::Log(Logger::WarningLevel::WARNING, "path " + aFilePath + " cound not be opened, the translations will not work");
        
        Logger::Log(Logger::WarningLevel::WARNING, "XML Error: " + std::string(Result.description()));
        
        IsInitialized = false;
        return false;
    }

    Logger::Log(Logger::WarningLevel::INFO, "Localizations file parsed succesfully");
    IsInitialized = true;

    return IsInitialized;
}

std::string LocalizationManager::GetTranslation(std::string aLabel)
{
    if (!IsInitialized || Doc.empty())
    {
        Logger::Log(Logger::WarningLevel::WARNING, "translations not working, check the path or the xml formatting");
        return aLabel;
    }

    pugi::xml_node LabelNode = Doc.child("localization").find_child_by_attribute("label", "id", aLabel.c_str());

    if (LabelNode)
    {
        std::string LanguageString = std::string(LanguageMapper::MapLanguageToString(CurrentLanguage));
        return LabelNode.attribute(LanguageString.c_str()).as_string(aLabel.c_str());
    }
    else
    {
        Logger::Log(Logger::WarningLevel::WARNING, "Label " + aLabel + " not found in the xml, check the label or the languages available");
    }

    return aLabel;
}

void LocalizationManager::SetLanguage(std::string aLanguage)
{
    CurrentLanguage = LanguageMapper::MapStringToLanguage(aLanguage);
}