#include <iostream>
#include <string>

class Logger
{
    public:
        enum class WarningLevel
        {
            INFO,
            WARNING
        };

        /**
        * @brief Function to handle terminal messages.
        * @param aWarning warning level of the message.
        * @param aMessage message to show in terminal.
        */
        static void Log(WarningLevel aWarning, std::string aMessage);
        
    private:
        static inline constexpr std::string_view RESET_COLOR = "\033[0m";           //!< standard console color.

        /**
        * @brief Gives a certain color depending of the warning level.
        * @param aWarning warning level of the message.
        * @return the associated color.
        */
        static std::string_view GetColor(WarningLevel aWarning);
};