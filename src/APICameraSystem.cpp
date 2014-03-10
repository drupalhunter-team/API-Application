#include "APICameraSystem.hpp"

using namespace kitrokopter;

/**
 * Initializes all cameras.
 *
 * See APICamera::initialize()
 *
 * @param quadcopters The quadcopters which will be tracked.
 */
void APICameraSystem::initializeCameras(std::vector<APIQuadcopter> quadcopters)
{
    for (std::vector<APICamera>::iterator it = cameras.begin(); it != cameras.end();
    ++it) {
        it->initialize(quadcopters);
    }
}

/**
 * @return A pointer to the camera with the given id.
 */
APICamera APICameraSystem::getCamera(int id)
{
    return cameras.at(id);
}

/**
 * @return The number of cameras in this system.
 */
int APICameraSystem::getCameraAmount()
{
    return cameras.size();
}

/**
 * @return A vector with all cameras.
 */
std::vector<APICamera> APICameraSystem::getCameras()
{
    return cameras;
}

/**
 * Finds all calibrated cameras.
 *
 * @return A vector of cameras.
 */
std::vector<APICamera> APICameraSystem::getCalibratedCameras()
{
    std::vector<APICamera> result;
    for (std::vector<APICamera>::iterator it = cameras.begin(); it != cameras.end(); ++it) {
	if (it->isCalibrated())
	    result.push_back(*it);
    }
    return result;
}

/**
 * Finds all uncalibrated cameras.
 *
 * @return A vector of cameras.
 */
std::vector<APICamera> APICameraSystem::getUncalibratedCameras()
{
    std::vector<APICamera> result;
    for (std::vector<APICamera>::iterator it = cameras.begin(); it != cameras.end(); ++it) {
	if (!it->isCalibrated())
	    result.push_back(*it);
    }
    return result;
}
