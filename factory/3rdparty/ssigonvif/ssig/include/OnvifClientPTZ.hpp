#ifndef ONVIFCLIENTPTZ_HPP
#define ONVIFCLIENTPTZ_HPP

#include "OnvifClientDevice.hpp"
#include "onvif/soapPTZBindingProxy.h"
#include<string>
class OnvifClientPTZ : public OnvifClientDevice{
public:
  OnvifClientPTZ(std::string url,
                 std::string user,
                 std::string password,
                 bool showCapabilities);
  ~OnvifClientPTZ();
  virtual std::string ErrorString();
  void getPTZConfigurations();
  void getStatus(std::string profileToken);
  void getConfiguration(std::string configurationToken);

  void absoluteMove(std::string profileToken,
                   float pan,
                   float panSpeed,
                   float tilt,
                   float tiltSpeed,
                   float zoom,
                   float zoomSpeed);

  void relativeMove(std::string profileToken,
                   float pan,
                   float panSpeed,
                   float tilt,
                   float tiltSpeed,
                   float zoom,
                   float zoomSpeed);

  void continuousMove(std::string profileToken,
                     float panSpeed,
                     float tiltSpeed,
                     float zoomSpeed);

  void stop(std::string profileToken,
           bool panTilt,
           bool zoom);
  
  void setHomePosition(std::string profileToken);
  void goToHomePosition(std::string profileToken);
  
  // pan to the left n Degress
  void panLeft(std::string profileToken, int nDegrees);
  // pan to the right n Degrees
  void panRight(std::string profileToken, int nDegrees);

  void tiltDown(std::string profileToken, int nDegrees);
  void tiltUp(std::string profileToken, int nDegrees);
  

	void gotoPreset(std::string profileToken,int PresetToken,float Speed);
	
	void getPresets(std::string profileToken);
private:
  PTZBindingProxy proxy_ptz_;
};

#endif
