#pragma once

#include "httplib.h"
#include <memory>

class LandmarkService;

class LandmarkController
{
public:

    /**
    * @brief class LandmarkController.
    * @param s httplib server.
    * @param svc landmarkservice implementation.
    */
    LandmarkController(httplib::Server& s, std::shared_ptr<LandmarkService> svc);

    /**
    * @brief Register the routes for the api.
    */
    void RegisterRoutes();

private:
    httplib::Server& Server;                    //!<server.
    std::shared_ptr<LandmarkService> Service;   //!<service.
};