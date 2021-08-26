#ifndef ONVIFCLIENTMEDIA_HPP
#define ONVIFCLIENTMEDIA_HPP

#include "OnvifClientDevice.hpp"
#include "onvif/soapMediaBindingProxy.h"
#include <string>

struct _ocp_Profile{
	std::string profileName;
	std::string profileToken;
};

class OnvifClientMedia : public OnvifClientDevice{
public:
  OnvifClientMedia(std::string url,
                   std::string user,
                   std::string password,
                   bool showCapabilities);
  ~OnvifClientMedia();
  virtual std::string ErrorString();
  void createProfile(std::string profileName,
                    std::string profileToken);
  void deleteProfile(std::string profileToken);
  void getProfiles(std::vector<_ocp_Profile> & profilesResponse );
  void getProfile(std::string profileToken);
  void addPTZConfiguration(std::string profileToken,
                           std::string configurationToken);
	//bool GetProfiles(std::vector<_ocp_Profile> & __profiles)
private:
  bool has_move_profile_;
  bool has_ptz_config_;
  MediaBindingProxy proxy_media_;
};

#endif
