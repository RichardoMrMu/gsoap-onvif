#include <chrono>
#include <iostream>
#include <sstream> 
#include "glog/logging.h"
#include "OnvifClientDevice.hpp"
#include "OnvifClientPTZ.hpp"
#include "OnvifClientMedia.hpp"

using namespace std;

int main(int argc, const char *argv[]) {
	
  std::string url = "192.168.66.64";
	std::string name = "admin";
	std::string password = "wst123456";
	OnvifClientPTZ *PTZ = new OnvifClientPTZ(url, name, password, true);
	OnvifClientMedia *Media = new OnvifClientMedia(url, name, password, true);
	std::vector<_ocp_Profile> get_profilesResponse;
	Media->getProfiles(get_profilesResponse);
	PTZ->gotoPreset(get_profilesResponse[0].profileToken,3,0.5);
	PTZ->getPresets(get_profilesResponse[0].profileToken);
return 0;
	// Media->deleteProfile("test");
	//std::cout << "Configurations:" << std::endl;
	// Media->addPTZConfiguration("Move", "PTZToken");
	//PTZ->getConfiguration("Profile_1");
	//PTZ->getPTZConfigurations();
	//PTZ->setHomePosition("Profile_1");
	// move
	//PTZ->continuousMove("Profile_1", 0.5, 0, 0);
	//sleep(2);
	//PTZ->stop("Profile_1", 0, 0);
	//PTZ->relativeMove("Profile_1", 0.0, 0.0, 0.0, 0.0, 0.5, 1);
	
	// zoom
	//PTZ->ZoomIn("Move");
	//sleep(2);
	//PTZ->ZoomOut("Move");
	//sleep(2);
	//PTZ->goToHomePosition("Move");
	//return 0;
	// move 
	

	
  	Media->createProfile("Move", "Move");
  	Media->addPTZConfiguration("Move", "PTZToken");
  	PTZ->getConfiguration("PTZToken");
	
	return 0;
}
