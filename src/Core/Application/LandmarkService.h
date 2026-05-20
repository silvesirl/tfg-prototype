#pragma once
#include <vector>
#include <memory>
#include <string>
#include <iostream>

#include "Landmark.h"
#include "ILandmarkDBRepository.h"
#include "DistanceMetricManager.h"
#include "DomainConstants.h"

class LandmarkService
{
public:
    /**
    * @brief class LandmarkService.
    * @param aRepo implementation of the chosen repository.
    * @param aMetricManager implementation of the metric manager.
    */
    LandmarkService(ILandmarkDBRepository& aRepo, std::shared_ptr<DistanceMetricManager> aMetricManager);

    /**
    * @brief Calculate the distance between two given landmarks.
    * @param aLocation1 point of origin.
    * @param aLocation2 point of destiny.
    * @return Calculated distance between the points.
    */
    double CalculateDistance(Landmark aLocation1, Landmark aLocation2);

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
    * @brief sets the filter for the type.
    * @param aFilteredType type to filter.
    */
    void SetFilteredType(std::string aFilteredType);

    /**
    * @brief sets the desired distance metric into the manager.
    * @param aDistanceMetricString text of the desired distance metric.
    */
    void SetMetricDistance(std::string aDistanceMetricString);

private:
    ILandmarkDBRepository& Repository;                                                  //!<repository to access the landmarks.
    std::shared_ptr<DistanceMetricManager> MetricManager;                               //!<Manager for the haversine algoritms and distance metrics.
    std::string FilteredContinent = std::string{DomainConstants::EMPTY_FILTER};         //!<filter in place for continent.
    std::string FilteredType = std::string{DomainConstants::EMPTY_FILTER};              //!<filter in place for types.
};