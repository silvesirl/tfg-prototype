#include <iostream>
#include "httplib.h"
#include "Logger.h"

// Core
#include "Landmark.h"
#include "LandmarkService.h"
#include "DistanceMetricManager.h"
#include "ILandmarkDBRepository.h"

// Infrastructure
#include "LandmarkController.h"
#include "SQLiteLandmarkRepository.h"
#include "Localization/LocalizationManager.h"

// Constants
constexpr uint32_t SERVER_PORT = 18080;
constexpr const char* SERVER_HOST = "0.0.0.0";

int main()
{
    try
    {
        httplib::Server Server;

        LocalizationManager::Initialize("Localization.xml");

        std::unique_ptr<ILandmarkDBRepository> DBRepository;

        DBRepository = std::make_unique<SQLiteLandmarkRepository>();

        auto MetricManager = std::make_shared<DistanceMetricManager>();

        auto LService = std::make_shared<LandmarkService>(*DBRepository, MetricManager);

        LandmarkController LController(Server, LService);

        LController.RegisterRoutes();

        Logger::Log(Logger::WarningLevel::INFO, std::format("Server listening to http:// {} : {} ...", SERVER_HOST, SERVER_PORT));
        
        if (!Server.listen(SERVER_HOST, SERVER_PORT))
        {
            throw std::runtime_error("Could not listen to port " + std::to_string(SERVER_PORT));
        }
    }
    catch(const std::exception& e)
    {
        std::cerr << "Fatal error: " << e.what() << std::endl;
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}