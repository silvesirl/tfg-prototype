#pragma once

#include <vector>
#include <memory>
#include <string>
#include "Landmark.h"
#include "ILandmarkDBRepository.h"
#include "IHaversineAlgorithmStrategy.h"
#include "HaversineKilometerAlgorithm.h"
#include "HaversineMilesAlgorithm.h"
#include "HaversineFeetAlgorithm.h"

class LandmarkService
{
public:
    /**
    * @brief class LandmarkService.
    * @param Repo implementation of the chosen repository.
    */
    LandmarkService(ILandmarkDBRepository& Repo);

    /**
    * @brief Calculate the distance between two given landmarks.
    * @param aLocation1 point of origin.
    * @param aLocation2 point of destiny.
    * @return Calculated distance between the points.
    */
    double CalculateHaversine(Landmark aLocation1, Landmark aLocation2);

    /**
    * @brief Getter of the landmarks taking into account the selected filters
    * @return a vector with the filtered landmarks.
    */
    std::vector<Landmark> GetProcessedLandmarks();

    /**
    * @brief sets the filter for the continent.
    * @param aFilteredContinent continet to filter.
    */
    void SetFilteredContinent(std::string aFilteredContinent);

    /**
    * @brief sets the filter for the category.
    * @param aFilteredCategory category to filter.
    */
    void SetFilteredCategory(std::string aFilteredCategory);

    /**
    * @brief sets the desired strategy to follow with the haversine algorithm.
    * @param aStrategy strategy to follow when calculating distances.
    */
    void SetHaversineStrategy(std::unique_ptr<IHaversineAlgorithmStrategy> aStrategy);

private:

    /**
    * @brief Pass a number into radias
    * @param aDegree degreees to convert to radians.
    * @return the radians.
    */
    double ToRadians(double aDegree);

    /**
    * @brief transform the type of landmark in string.
    * @param aLandmarkType type to convert.
    * @return type converted.
    */
    LandmarkType TransformStringToType(std::string aLandmarkType);


    std::unique_ptr<IHaversineAlgorithmStrategy> HaversineStrategy; //!<haversinestrategy chosen.
    ILandmarkDBRepository& Repository;                              //!<repository to access the landmarks.
    std::string FilteredContinent = "-";                            //!<filter in place for continent.
    std::string FilteredCategory = "-";                             //!<filter in place for categories.
};