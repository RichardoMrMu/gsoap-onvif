#include "OnvifClientMedia.hpp"
#include "glog/logging.h"

OnvifClientMedia::OnvifClientMedia(std::string url, 
                                   std::string user, 
                                   std::string password, 
                                   bool showCapabilities) 
  : OnvifClientDevice(url, user, password, showCapabilities),
    has_move_profile_(false), has_ptz_config_(false) {
  if(has_media_) {
    proxy_media_.soap_endpoint = media_url_.c_str();
    soap_register_plugin(proxy_media_.soap, soap_wsse);
  }else{
    LOG(FATAL) << "Camera didn't implement Media functions";
  }
}

OnvifClientMedia::~OnvifClientMedia() {}

void OnvifClientMedia::getProfile(std::string profileToken) {
  CHECK_EQ(SOAP_OK, soap_wsse_add_UsernameTokenDigest(proxy_media_.soap, NULL, 
    user_.c_str(), passwd_.c_str())) << "wsse error";
  auto *profile = soap_new__trt__GetProfile(soap_, -1);
  auto *profileResponse = soap_new__trt__GetProfileResponse(soap_, -1);

  profile->ProfileToken = profileToken;
  CHECK_EQ(SOAP_OK, proxy_media_.GetProfile(profile, profileResponse))
    << "get profile error " << ErrorString();

  LOG(INFO) << "Video Source token: " 
    << profileResponse->Profile->VideoSourceConfiguration->token;
  LOG(INFO) << "Video Encoder token: " 
    << profileResponse->Profile->VideoEncoderConfiguration->token;

  soap_destroy(soap_); 
  soap_end(soap_);
}

void OnvifClientMedia::createProfile(std::string profileName,
                                     std::string profileToken) {
  //getProfiles(profileToken);
  if (!has_move_profile_) {
    CHECK_EQ(SOAP_OK, soap_wsse_add_UsernameTokenDigest(proxy_media_.soap, NULL, 
    user_.c_str(), passwd_.c_str())) << "wsse error";
    auto *cr_profile = soap_new__trt__CreateProfile(soap_, -1);
    auto *cr_profileResponse = soap_new__trt__CreateProfileResponse(soap_, -1);

    cr_profile->Name = profileName;
    cr_profile->Token = &profileToken;
    CHECK_EQ(SOAP_OK, proxy_media_.CreateProfile(cr_profile, cr_profileResponse))
      << "create profile error " << ErrorString();
    
    soap_destroy(soap_); 
    soap_end(soap_);
  } else {
    LOG(WARNING) << "Already has this profile, doesn't need to create again";
  }
}

void OnvifClientMedia::deleteProfile(std::string profileToken) {
  CHECK_EQ(SOAP_OK, soap_wsse_add_UsernameTokenDigest(proxy_media_.soap, NULL, 
    user_.c_str(), passwd_.c_str())) << "wsse error";
  auto *del_profile = soap_new__trt__DeleteProfile(soap_, -1);
  auto *del_profileResponse = soap_new__trt__DeleteProfileResponse(soap_, -1);

  del_profile->ProfileToken = profileToken;
  CHECK_EQ(SOAP_OK, proxy_media_.DeleteProfile(del_profile, del_profileResponse))
    << "delete profile error " << ErrorString();
  
  soap_destroy(soap_); 
  soap_end(soap_);
}

void  OnvifClientMedia::addPTZConfiguration(std::string profileToken,
                                           std::string configurationToken) {
  CHECK_EQ(SOAP_OK, soap_wsse_add_UsernameTokenDigest(proxy_media_.soap, NULL, 
    user_.c_str(), passwd_.c_str())) << "wsse error";
  auto *add_config = soap_new__trt__AddPTZConfiguration(soap_, -1);
  auto *add_configResponse = soap_new__trt__AddPTZConfigurationResponse(soap_, -1);

  add_config->ProfileToken = profileToken;	
  add_config->ConfigurationToken = configurationToken;
  CHECK_EQ(SOAP_OK, proxy_media_.AddPTZConfiguration(add_config, add_configResponse))
    << "add ptz config error " << ErrorString();

  soap_destroy(soap_); 
  soap_end(soap_);
}
/*
bool OnvifClientMedia::GetProfiles(std::vector<_ocp_Profile> & __profiles) {

	if (SOAP_OK != soap_wsse_add_UsernameTokenDigest(proxyMedia.soap, NULL, _ocp_username.c_str(), _ocp_password.c_str())) {
//		PrintErr(proxyMedia.soap);
		return false;
	}

	struct soap* soap = soap_new();
	_trt__GetProfiles * request = soap_new__trt__GetProfiles(soap);
	_trt__GetProfilesResponse * response = soap_new__trt__GetProfilesResponse(soap);

	bool execute_result = true;

		__profiles.clear();
	if (SOAP_OK == proxyMedia.GetProfiles(request,response)) {
		for (int i = 0; i < response->Profiles.size(); ++i) {
			_ocp_Profile profile; 
			profile.profileName = response->Profiles[i]->Name;
			profile.profileToken = response->Profiles[i]->token;
			__profiles.emplace_back(profile);
		}
		execute_result = true;
	} else {
//		PrintErr(proxyMedia.soap);
		execute_result = false;
	}

	soap_destroy(soap);
	soap_end(soap);
	return execute_result;
}

*/
void OnvifClientMedia::getProfiles(std::vector<_ocp_Profile> & profilesResponse ) {
  CHECK_EQ(SOAP_OK, soap_wsse_add_UsernameTokenDigest(proxy_media_.soap, NULL, 
    user_.c_str(), passwd_.c_str())) << "wsse error";
  CHECK_EQ(SOAP_OK, soap_wsse_add_Timestamp(proxy_media_.soap, "Time", 10))
    << "wsse add timestamp error";

	struct soap* soap = soap_new();
	_trt__GetProfiles * get_profiles = soap_new__trt__GetProfiles(soap);
	_trt__GetProfilesResponse * get_profilesResponse = soap_new__trt__GetProfilesResponse(soap);

  //auto *get_profiles = soap_new__trt__GetProfiles(soap_, -1);
  //auto *get_profilesResponse = soap_new__trt__GetProfilesResponse(soap_, -1);

  CHECK_EQ(SOAP_OK, proxy_media_.GetProfiles(get_profiles, get_profilesResponse))
    << "get profiles error " << ErrorString();
/*
  auto *getstream_uri = soap_new__trt__GetStreamUri(soap_, -1);
  getstream_uri->StreamSetup = soap_new_tt__StreamSetup(soap_, -1);
  getstream_uri->StreamSetup->Stream = tt__StreamType__RTP_Unicast;
  getstream_uri->StreamSetup->Transport = soap_new_tt__Transport(soap_, -1);
  getstream_uri->StreamSetup->Transport->Protocol = tt__TransportProtocol__RTSP;

  auto *getstream_uriResponse = soap_new__trt__GetStreamUriResponse(soap_, -1);
*/
/*
    getstream_uri->ProfileToken = get_profilesResponse->Profiles[i]->token;
    CHECK_EQ(SOAP_OK, soap_wsse_add_UsernameTokenDigest(proxy_media_.soap, NULL,
      user_.c_str(), passwd_.c_str())) << "wsse error";
    
    CHECK_EQ(SOAP_OK, proxy_media_.GetStreamUri(getstream_uri, getstream_uriResponse))
      << "get profiles error " << ErrorString();
 */

	profilesResponse.clear();
		for (int i = 0; i < get_profilesResponse->Profiles.size(); ++i) {
LOG(INFO) << " profile : " << get_profilesResponse->Profiles[i]->Name
              << " Token : " << get_profilesResponse->Profiles[i]->token;
			_ocp_Profile profile; 
			profile.profileName = get_profilesResponse->Profiles[i]->Name;
			profile.profileToken = get_profilesResponse->Profiles[i]->token;
			profilesResponse.emplace_back(profile);
		}

  soap_destroy(soap_); 
  soap_end(soap_); 
}

std::string OnvifClientMedia::ErrorString() {
  std::string result = "";
  result += std::to_string(proxy_media_.soap->error);
  result += " FaultString : ";
  if (*soap_faultstring(proxy_media_.soap)) {
    std::string faultstring(*soap_faultstring(proxy_media_.soap));
    result += faultstring;
  } else {
    result += "null";
  }
  result += " FaultCode : ";
  if (*soap_faultcode(proxy_media_.soap)) {
    std::string faultcode(*soap_faultcode(proxy_media_.soap));
    result += faultcode;
  } else {
    result += "null";
  }
  result += " FaultSubcode : ";
  if (*soap_faultsubcode(proxy_media_.soap)) {
    std::string faultsubcode(*soap_faultsubcode(proxy_media_.soap));
    result += faultsubcode;
  } else {
    result += "null";
  }
  result += " FaultDetail : ";
  if (*soap_faultdetail(proxy_media_.soap)) {
    std::string faultdetail(*soap_faultdetail(proxy_media_.soap));
    result += faultdetail;
  } else {
    result += "null";
  }
  return result;
}
