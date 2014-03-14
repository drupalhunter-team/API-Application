#include "APICamera.hpp"

// Messages
#include "camera_application/InitializeCameraService.h"
#include "camera_application/PictureSendingActivation.h"
#include "camera_application/CalibrateCamera.h"

#include <string>
#include <stdexcept>

using namespace kitrokopter;

/**
 * Constructs an APICamera.
 * 
 * @param newId the cameras id
 */
APICamera::APICamera(uint32_t newId) : calibrated(false),
			 calibration(NULL)
{
    ros::NodeHandle nh;
    this->id = newId;
    nh.subscribe("Picture", 1, &APICamera::handlePicture, this);
}

/**
 * Destructs an APICamera.
 */
APICamera::~APICamera()
{
    if (calibration)
	delete calibration;

    // Delete calibration images.
    for (std::vector<cv::Mat*>::iterator it = calibrationImages.begin(); it != calibrationImages.end();
    ++it) {
        delete *it;
    }
}

/**
 * Starts the calibration process.
 *
 * @param imageAmount The number of pictures to take.
 * @param waitingTime The time to wait between images in ms.
 */
void APICamera::startCalibration(int imageAmount, int waitingTime, const CalibrationBoard &board)
{
    ros::NodeHandle nh;
    ros::Publisher pub = nh.advertise<camera_application::CalibrateCamera>("CalibrateCamera", 1);
    camera_application::CalibrateCamera msg;
    msg.ID = this->id;
    msg.imageAmount = imageAmount;
    msg.imageDelay = waitingTime;
    msg.boardWidth = board.width;
    msg.boardHeight = board.height;
    msg.boardRectangleWidth = board.rectangleWidth;
    msg.boardRectangleHeight = board.rectangleHeight;
    pub.publish(msg);
}

/**
 * Sets calibration data.
 *
 * @param data The calibration data to set.
 */
void APICamera::setCalibrationData(APICalibrationData data)
{
    if (calibration)
	*calibration = data;
    else
	calibration = new APICalibrationData(data);
}

/**
 * @return The calibration data.
 */
APICalibrationData* const APICamera::getCalibrationData()
{
    return calibration;
}

/**
 * @return Whether the camera is calibrated.
 */
bool APICamera::isCalibrated()
{
    return calibrated;
}

/**
 * Deletes this camera's calibration.
 */
void APICamera::deleteCalibration()
{
    calibrated = false;
}

/**
 * Adds an image listener.
 *
 * The image listener will receive images this camera takes.
 *
 * @param listener A pointer to the listener.
 */
void APICamera::addImageListener(APIImageListener *listener)
{
    imageListeners.push_back(listener);
}

/**
 * Adds an camera listener.
 *
 * @param listener A pointer to the listener.
 */
void APICamera::addCameraListener(APICameraListener *listener)
{
    cameraListeners.push_back(listener);
}

template <typename T>
void removeListener(std::vector<T*> from, T *listener)
{
    for (typename std::vector<T*>::iterator it = from.begin(); it != from.end(); ++it) {
	if (*it == listener) {
	    from.erase(it);
	    return;
	}
    }
}

/**
 * Removes an image listener.
 *
 * Remember to free it!
 *
 * @param listener A pointer to the listener.
 */
void APICamera::removeImageListener(APIImageListener *listener)
{
    removeListener(imageListeners, listener);
}

/**
 * Removes an camera listener.
 *
 * Remember to free it!
 *
 * @param listener A pointer to the listener.
 */
void APICamera::removeCameraListener(APICameraListener *listener)
{
    removeListener(cameraListeners, listener);
}

/**
 * Sends a message requesting pictures to be or not to be sent.
 *
 * @param active `true` will enable image sending, `false` will disable.
 */
void APICamera::sendPictureSendingActivation(bool active)
{
    ros::NodeHandle nh;
    ros::Publisher pub = nh.advertise<camera_application::PictureSendingActivation>("PictureSendingActivation", 1);
    camera_application::PictureSendingActivation msg;
    msg.ID = this->id;
    msg.active = active;
    pub.publish(msg);
}

cv::Mat* msgToMat(camera_application::Picture::_image_type data)
{
    cv::Mat *mat = new cv::Mat(cv::Size(640, 480), CV_8UC3);
    for (size_t i = 0; i < (640 * 480 * 3); i++) {
	mat->data[i] = data[i];
    }
    return mat;
}

/**
 * Handles incoming pictures.
 */
void APICamera::handlePicture(const camera_application::Picture::Ptr &msg)
{
    if (msg->ID != this->id) return;
    if (msg->calibrationImage) {
	// Save the image.
	calibrationImages.push_back(msgToMat(msg->image));
    }
}
