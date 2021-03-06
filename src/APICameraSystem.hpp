#pragma once

#include <vector>
#include <stdint.h>
#include <opencv2/core/core.hpp>

#include "APICamera.hpp"

// Messages
#include "camera_application/InitializeCameraService.h"
#include "camera_application/Picture.h"

namespace kitrokopter {
class APICameraSystem {
public:
	APICameraSystem();

	/* Initialization */
	void initializeCameras(const std::map<uint32_t, APIQuadcopter> &quadcopters);

	/* Camera getters */
	APICamera* getCamera(int id);
	int getCameraAmount();

	std::map<uint32_t, APICamera>* getCamerasAsMap();

	/* Camera setters */
	void addCamera(APICamera camera);

	/* Calibration */
	bool startCalibration(const CalibrationBoard &board);

	std::map<uint32_t, bool> takeCalibrationPictures();
	void calculateCalibration();

	/* Calibration getters */
	bool isCalibrated();

	std::vector<APICamera*> getCamerasAsVector();

	std::vector<APICamera*> getCalibratedCameras();

	std::vector<APICamera*> getUncalibratedCameras();

	std::vector<std::vector<cv::Mat> > getCalibrationPictures();
	int getCalibrationPictureCount();

private:
	std::map<uint32_t, APICamera> cameras;
	camera_application::InitializeCameraService buildInitMessage(const std::map<uint32_t, APIQuadcopter> &quadcopters);
};
}
