#include "ros/ros.h"
#include <ros/console.h>
#include "api_application/Announce.h"

#include <vector> 

class API
{
    public:
	API(int argc, char **argv):
	{
		ros::init(argc, argv, "api_server");
		ros::NodeHandle n;
		ros::ServiceServer service = n.advertiseService("announce", announce);
		ROS_INFO("Ready to deliver IDs.");
		ros::spin();
		return 0;
	}

    private:
	int idCounter = 0;
	
	//the ids of modules by category
	vector<int[2]> cameras; //first value is the module id, second the camera id
	vector<int> quadcopters;
	vector<int> controllers;
	vector<int> positions;
	
	bool announce(camera_application::Announce::Request  &req,
		camera_application::Announce::Response &res)
	{
	    res.id = idCounter++;
	    switch (req.type) {
		case 0:
		    cameras.push_back({res.id, req.camera_id});
		    break;
		case 1:
		    quadcopters.push_back(res.id);
		    break;
		case 2:
		    controllers.push_back(res.id);
		    break;
		case 3:
		    positions.push_back(res.id);
		    break;
		default:
		    ROS_ERROR("Malformed register attempt!");
		    res.id = -1;
		    return false;

	    ROS_INFO("Registered new module with type %d and id %d", req.type, res.id);
	    return true;
	}
}    