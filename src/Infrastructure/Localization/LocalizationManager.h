#pragma once
#include <string>
#include "Language.h"
#include "pugixml.hpp"

class LocalizationManager
{
public:

    /**
    * @brief Initialize the Localization manager by reading the xml.
    * @param aFilePath route of the xml to read.
    * @return Boolean to confirm if the file was readed succesfully or not.
    */
    static bool Initialize(const std::string& aFilePath);

    /**
    * @brief Get the result in the xml with a given label, to be succesulfull we first need to initialize the Localization manager.
    * @param aLabel label to get the result depending of the language in the xml.
    * @return the result string.
    */
    static std::string GetTranslation(std::string aLabel);

    /**
    * @brief Sets the language of the manager.
    * @param aLanguage Language in string to change to.
    */
    static void SetLanguage(std::string aLanguage);

private:
    static pugi::xml_document Doc;                      //!<Result of the given file to read.
    static bool IsInitialized;                          //!<If the manager is initialized or not.
    static Language CurrentLanguage;                    //!<Current language chosen.
};