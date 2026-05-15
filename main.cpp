#include <iostream>
#include <cmath>

#include "Landmark.h"
#include "httplib.h"

#include "LandmarkController.h"
#include "LandmarkService.h"
#include "ILandmarkDBRepository.h"
#include "SQLiteLandmarkRepository.h"

std::vector<Landmark> LandmarkList;

std::unique_ptr<ILandmarkDBRepository> DBRepository;

int main()
{
    httplib::Server Server;

    DBRepository = std::unique_ptr<ILandmarkDBRepository>(new SQLiteLandmarkRepository());

    auto LService = std::make_shared<LandmarkService>(*DBRepository);

    LandmarkController LController(Server, LService);

    LController.RegisterRoutes();

    std::cout << "Server listening to http://localhost:18080..." << std::endl;
    Server.listen("0.0.0.0", 18080);

    return 0;
}