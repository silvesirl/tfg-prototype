#include "Logger.h"

std::string_view Logger::GetColor(WarningLevel aWarning)
{
    switch(aWarning)
    {
        case WarningLevel::INFO:
            return "\033[36m";
            break;
        
        case WarningLevel::WARNING:
            return "\033[33m";
            break;

        default:
            return RESET_COLOR;
    }
}

void Logger::Log(WarningLevel aWarning, std::string aMessage)
{
    std::cerr << GetColor(aWarning);

    switch(aWarning)
    {
        case WarningLevel::INFO:
            std::cerr << "[INFO]";
            break;

        case WarningLevel::WARNING:
            std::cerr << "[WARNING]";
            break;
    }

    std::cerr << aMessage << RESET_COLOR << std::endl;
}