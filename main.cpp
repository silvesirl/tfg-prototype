#include <iostream>
#include <cmath>
#include "httplib.h"

// Core
#include "Landmark.h"

// Infrastructure
#include "LandmarkController.h"
#include "LandmarkService.h"
#include "ILandmarkDBRepository.h"
#include "SQLiteLandmarkRepository.h"

// Constants
constexpr int SERVER_PORT = 18080;
const std::string SERVER_HOST = "0.0.0.0";

std::unique_ptr<ILandmarkDBRepository> DBRepository;

int main()
{
    try
    {
        httplib::Server Server;

        DBRepository = std::make_unique<SQLiteLandmarkRepository>();

        auto LService = std::make_shared<LandmarkService>(*DBRepository);

        LandmarkController LController(Server, LService);

        LController.RegisterRoutes();

        std::cout << "Server listening to http://" << SERVER_HOST << ":" << SERVER_PORT << "..." << std::endl;
        
        if (!Server.listen(SERVER_HOST.c_str(), SERVER_PORT))
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