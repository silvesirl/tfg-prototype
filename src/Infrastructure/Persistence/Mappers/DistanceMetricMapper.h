#pragma once

#include <string_view>
#include <stdexcept>
#include "DistanceMetric.h"

namespace DistanceMetricMapper
{
    inline constexpr std::string_view KILOMETER_STRING = "Kilometers";
    inline constexpr std::string_view MILES_STRING = "Miles";
    inline constexpr std::string_view FEET_STRING = "Feet";

    /**
    * @brief Converts a string representation of a distance metric into its corresponding enum.
    * @param aDistanceMetricString The string_view to convert.
    * @return The corresponding enum.
    * @throws std::invalid_argument If the string does not match any valid metric.
    */
    static DistanceMetric MapStringToDistanceMetric(std::string_view aDistanceMetricString)
    {
        if (aDistanceMetricString == KILOMETER_STRING)
        {
            return DistanceMetric::KILOMETERS;
        }
        if (aDistanceMetricString == MILES_STRING)
        {
            return DistanceMetric::MILES;
        }
        if (aDistanceMetricString == FEET_STRING)
        {
            return DistanceMetric::FOOT;
        }

        throw std::invalid_argument("Invalid distance metric string: " + std::string(aDistanceMetricString));
    }

    /**
    * @brief Converts a DistanceMetric enum into its corresponding string_view representation.
    * @param aDistanceMetric The enum value to convert.
    * @return text representing the metric name.
    * @throws std::invalid_argument If the enum value is unknown.
    */
    static std::string_view MapDistanceMetricToString(DistanceMetric aDistanceMetric)
    {
        switch (aDistanceMetric)
        {
            case DistanceMetric::KILOMETERS: return KILOMETER_STRING;
            case DistanceMetric::MILES:      return MILES_STRING;
            case DistanceMetric::FOOT:       return FEET_STRING;
            default:                         throw std::invalid_argument("Unknown DistanceMetric enum value");
        }
    }
}