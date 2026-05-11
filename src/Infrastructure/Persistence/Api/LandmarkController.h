#pragma once

#include "httplib.h"
#include <memory>

class LandmarkService;

class LandmarkController
{
public:
    LandmarkController(httplib::Server& s, std::shared_ptr<LandmarkService> svc);

    void RegisterRoutes();

private:
    httplib::Server& Server;
    std::shared_ptr<LandmarkService> Service;
};