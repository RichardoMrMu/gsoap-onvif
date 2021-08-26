#include "OnvifClientPTZ.hpp"
#include "glog/logging.h"

OnvifClientPTZ::OnvifClientPTZ(std::string url,
                               std::string user,
                               std::string password,
                               bool showCapabilities)
  : OnvifClientDevice(url, user, password, showCapabilities) {
  if(has_ptz_) {
    proxy_ptz_.soap_endpoint = ptz_url_.c_str();
 LOG(INFO) <<  "proxy_ptz_.soap_endpoint:"<<proxy_ptz_.soap_endpoint;
    soap_register_plugin(proxy_ptz_.soap, soap_wsse);
  } else {
    LOG(FATAL) <<  "Camera does not implement PTZ functions";
  }
}

OnvifClientPTZ::~OnvifClientPTZ() {}

void OnvifClientPTZ::getPTZConfigurations() {
  CHECK_EQ(SOAP_OK, soap_wsse_add_UsernameTokenDigest(proxy_ptz_.soap, NULL, 
    user_.c_str(), passwd_.c_str())) << "wsse error";
  
  auto *configs = soap_new__tptz__GetConfigurations(soap_, -1);
  auto *configresponse = soap_new__tptz__GetConfigurationsResponse(soap_, -1);
  CHECK_EQ(SOAP_OK, proxy_ptz_.GetConfigurations(configs, configresponse))
    << "get ptz configurations error " << ErrorString();
  
  for(int i = 0; i < configresponse->PTZConfiguration.size(); ++i) {
    LOG(INFO) << "ConfigurationToken #" << i << ": " 
              << configresponse->PTZConfiguration[i]->token;
  }
  soap_destroy(soap_);
  soap_end(soap_);
}

void OnvifClientPTZ::getStatus(std::string profileToken) {
  CHECK_EQ(SOAP_OK, soap_wsse_add_UsernameTokenDigest(proxy_ptz_.soap, NULL, 
    user_.c_str(), passwd_.c_str())) << "wsse error";

  auto *status = soap_new__tptz__GetStatus(soap_, -1);
  auto *status_response = soap_new__tptz__GetStatusResponse(soap_, -1);

  status->ProfileToken = profileToken.c_str();
  CHECK_EQ(SOAP_OK, proxy_ptz_.GetStatus(status, status_response))
    << "get status error " << ErrorString();
  
  LOG(INFO) << "PAN: " << status_response->PTZStatus->Position->PanTilt->x; 
  LOG(INFO)	<< "TILT: " <<status_response->PTZStatus->Position->PanTilt->y;
  LOG(INFO) << "ZOOM:" << status_response->PTZStatus->Position->Zoom->x;
  LOG(INFO) << "PanTilt:" << status_response->PTZStatus->MoveStatus->PanTilt; 
  LOG(INFO) << "ZoomStatus" << status_response->PTZStatus->MoveStatus->Zoom; 
  LOG(INFO) << "ERROR: " << status_response->PTZStatus->Error; 
  LOG(INFO) << "UtcTime: " << status_response->PTZStatus->UtcTime;
  soap_destroy(soap_); 
  soap_end(soap_);
}
void OnvifClientPTZ::gotoPreset(std::string profileToken,
                                  int PresetToken,
                                  float Speed) {
  CHECK_EQ(SOAP_OK, soap_wsse_add_UsernameTokenDigest(proxy_ptz_.soap, NULL, 
    user_.c_str(), passwd_.c_str())) << "wsse error";
	auto *go_preset = soap_new__tptz__GotoPreset(soap_, -1);
  auto *go_presetResponse = soap_new__tptz__GotoPresetResponse(soap_, -1);
	int dwPresetIndex = 1;	
	std::string strPresetToken = std::to_string(PresetToken);
  go_preset->ProfileToken = profileToken;
	go_preset->PresetToken = strPresetToken;
  CHECK_EQ(SOAP_OK, proxy_ptz_.GotoPreset(go_preset, go_presetResponse))
    << " goto preset error" << ErrorString();
  
  soap_destroy(soap_); 
  soap_end(soap_);
}

void OnvifClientPTZ::getPresets(std::string profileToken) {
 	CHECK_EQ(SOAP_OK, soap_wsse_add_UsernameTokenDigest(proxy_ptz_.soap, NULL, 
    	user_.c_str(), passwd_.c_str())) << "wsse error";
	struct soap* soap = soap_new();
	_tptz__GetPresets * get_presets = soap_new__tptz__GetPresets(soap);
	_tptz__GetPresetsResponse * get_presetsResponse = soap_new__tptz__GetPresetsResponse(soap);
	get_presets->ProfileToken=profileToken.c_str();
	CHECK_EQ(SOAP_OK, proxy_ptz_.GetPresets(get_presets, get_presetsResponse))
    	<< "get profiles error " << ErrorString();
	LOG(INFO)<<get_presetsResponse;

	for (int i = 0; i < get_presetsResponse->Preset.size(); ++i) {
			LOG(INFO) << " profile : " << get_presetsResponse->Preset[i]->Name
              << " Token : " << get_presetsResponse->Preset[i]->token;}

/*
			_ocp_Profile profile; 
			profile.profileName = get_profilesResponse->Profiles[i]->Name;
			profile.profileToken = get_profilesResponse->Profiles[i]->token;
			profilesResponse.emplace_back(profile);
*/
		

 	soap_destroy(soap_); 
  	soap_end(soap_);

}
void OnvifClientPTZ::absoluteMove(std::string profileToken,
                                  float pan,
                                  float panSpeed,
                                  float tilt,
                                  float tiltSpeed,
                                  float zoom,
                                  float zoomSpeed) {
  CHECK_EQ(SOAP_OK, soap_wsse_add_UsernameTokenDigest(proxy_ptz_.soap, NULL, 
    user_.c_str(), passwd_.c_str())) << "wsse error";

  auto *abs_move = soap_new__tptz__AbsoluteMove(soap_, -1);
  auto *abs_moveResponse = soap_new__tptz__AbsoluteMoveResponse(soap_, -1);

  abs_move->ProfileToken = profileToken;

  //setting pan and tilt
  abs_move->Position = soap_new_tt__PTZVector(soap_, -1);
  abs_move->Position->PanTilt = soap_new_tt__Vector2D(soap_, -1);
  abs_move->Speed = soap_new_tt__PTZSpeed(soap_, -1);
  abs_move->Speed->PanTilt = soap_new_tt__Vector2D(soap_, -1);
  //pan
  abs_move->Position->PanTilt->x = pan;
  abs_move->Speed->PanTilt->x = panSpeed;
  //tilt
  abs_move->Position->PanTilt->y = tilt;
  abs_move->Speed->PanTilt->y = tiltSpeed;
  //setting zoom
  abs_move->Position->Zoom = soap_new_tt__Vector1D(soap_, -1);
  abs_move->Speed->Zoom = soap_new_tt__Vector1D(soap_, -1);
  abs_move->Position->Zoom->x = zoom;
  abs_move->Speed->Zoom->x = zoomSpeed;

  CHECK_EQ(SOAP_OK, proxy_ptz_.AbsoluteMove(abs_move, abs_moveResponse))
    << "absoulte move error " << ErrorString();

  soap_destroy(soap_); 
  soap_end(soap_);
}

void OnvifClientPTZ::relativeMove(std::string profileToken,
                                  float pan,
                                  float panSpeed,
                                  float tilt,
                                  float tiltSpeed,
                                  float zoom,
                                  float zoomSpeed) {
  CHECK_EQ(SOAP_OK, soap_wsse_add_UsernameTokenDigest(proxy_ptz_.soap, NULL, 
    user_.c_str(), passwd_.c_str())) << "wsse error";
  
  auto *rlt_move = soap_new__tptz__RelativeMove(soap_, -1);
  auto *rlt_moveResponse = soap_new__tptz__RelativeMoveResponse(soap_, -1);

  rlt_move->ProfileToken = profileToken;

  //setting pan and tilt
  rlt_move->Translation = soap_new_tt__PTZVector(soap_, -1);
  rlt_move->Translation->PanTilt = soap_new_tt__Vector2D(soap_, -1);
  rlt_move->Speed = soap_new_tt__PTZSpeed(soap_, -1);
  rlt_move->Speed->PanTilt = soap_new_tt__Vector2D(soap_, -1);
  //pan
  rlt_move->Translation->PanTilt->x = pan;
  rlt_move->Speed->PanTilt->x = panSpeed;
  //tilt
  rlt_move->Translation->PanTilt->y = tilt;
  rlt_move->Speed->PanTilt->y = tiltSpeed;
  //setting zoom
  rlt_move->Translation->Zoom = soap_new_tt__Vector1D(soap_, -1);
  rlt_move->Speed->Zoom = soap_new_tt__Vector1D(soap_, -1);
  rlt_move->Translation->Zoom->x = zoom;
  rlt_move->Speed->Zoom->x = zoomSpeed;

proxy_ptz_.RelativeMove(rlt_move, rlt_moveResponse);
/*
  CHECK_EQ(SOAP_OK, proxy_ptz_.RelativeMove(rlt_move, rlt_moveResponse))
    << "relative move error " << ErrorString();
*/
  soap_destroy(soap_); 
  soap_end(soap_);
}

void OnvifClientPTZ::continuousMove(std::string profileToken,
                                    float panSpeed,
                                    float tiltSpeed,
                                    float zoomSpeed) {
  CHECK_EQ(SOAP_OK, soap_wsse_add_UsernameTokenDigest(proxy_ptz_.soap, NULL, 
    user_.c_str(), passwd_.c_str())) << "wsse error";

  auto *cnt_move = soap_new__tptz__ContinuousMove(soap_, -1);
  auto *cnt_moveResponse = soap_new__tptz__ContinuousMoveResponse(soap_, -1);

  cnt_move->ProfileToken = profileToken;

  //setting pan and tilt speed
  cnt_move->Velocity = soap_new_tt__PTZSpeed(soap_, -1);
  cnt_move->Velocity->PanTilt = soap_new_tt__Vector2D(soap_, -1);
  cnt_move->Velocity->PanTilt->x = panSpeed;
  cnt_move->Velocity->PanTilt->y = tiltSpeed;

  //setting zoom speed
  cnt_move->Velocity->Zoom = soap_new_tt__Vector1D(soap_, -1);
  cnt_move->Velocity->Zoom->x = zoomSpeed;

  int64_t tempo = 1;
  cnt_move->Timeout = &tempo;

  CHECK_EQ(SOAP_OK, proxy_ptz_.ContinuousMove(cnt_move, cnt_moveResponse))
    << "relative move error " << ErrorString();

  soap_destroy(soap_); 
  soap_end(soap_);
}

void OnvifClientPTZ::stop(std::string profileToken,
                          bool panTilt,
                          bool zoom) {
  CHECK_EQ(SOAP_OK, soap_wsse_add_UsernameTokenDigest(proxy_ptz_.soap, NULL, 
    user_.c_str(), passwd_.c_str())) << "wsse error";

  auto *stop= soap_new__tptz__Stop(soap_, -1);
  auto *stop_response = soap_new__tptz__StopResponse(soap_, -1);

  stop->ProfileToken = profileToken;
  stop->PanTilt = &panTilt;
  stop->Zoom = &zoom;

  CHECK_EQ(SOAP_OK, proxy_ptz_.Stop(stop, stop_response))
    << "stop error" << ErrorString();

  soap_destroy(soap_); 
  soap_end(soap_);
}

void OnvifClientPTZ::setHomePosition(std::string profileToken) {
  CHECK_EQ(SOAP_OK, soap_wsse_add_UsernameTokenDigest(proxy_ptz_.soap, NULL, 
    user_.c_str(), passwd_.c_str())) << "wsse error";

  auto *set_home = soap_new__tptz__SetHomePosition(soap_, -1);
  auto *set_homeResponse = soap_new__tptz__SetHomePositionResponse(soap_, -1);

  set_home->ProfileToken = profileToken;
proxy_ptz_.SetHomePosition(set_home, set_homeResponse);
/*
  CHECK_EQ(SOAP_OK, proxy_ptz_.SetHomePosition(set_home, set_homeResponse))
    << "set home position error" << ErrorString();
*/
  soap_destroy(soap_); 
  soap_end(soap_);
}

void OnvifClientPTZ::goToHomePosition(std::string profileToken){
  CHECK_EQ(SOAP_OK, soap_wsse_add_UsernameTokenDigest(proxy_ptz_.soap, NULL, 
    user_.c_str(), passwd_.c_str())) << "wsse error";

  auto *go_home = soap_new__tptz__GotoHomePosition(soap_, -1);
  auto *go_homeResponse = soap_new__tptz__GotoHomePositionResponse(soap_, -1);

  go_home->ProfileToken = profileToken;

  CHECK_EQ(SOAP_OK, proxy_ptz_.GotoHomePosition(go_home, go_homeResponse))
    << " go home error" << ErrorString();
  
  soap_destroy(soap_); 
  soap_end(soap_);
}

void OnvifClientPTZ::getConfiguration(std::string configurationToken){
  CHECK_EQ(SOAP_OK, soap_wsse_add_UsernameTokenDigest(proxy_ptz_.soap, NULL, 
    user_.c_str(), passwd_.c_str())) << "wsse error";

  auto *config = soap_new__tptz__GetConfiguration(soap_,-1);
  auto *configResponse = soap_new__tptz__GetConfigurationResponse(soap_,-1);

  config->PTZConfigurationToken = configurationToken;
  CHECK_EQ(SOAP_OK, proxy_ptz_.GetConfiguration(config, configResponse)) 
    << " get configuration error" << ErrorString();
  /*for (int i = 0; i < configResponse->PTZConfiguration.size(); i++) {
	//for (int i = 0; i < 12; i++) {
    LOG(INFO) << " profile : " << configResponse->PTZConfiguration[i].Name
              << " Token : " << configResponse->PTZConfiguration[i].token;

	}*/
  LOG(INFO) << "Absolute Pan Tilt Position Space: " 
    << configResponse->PTZConfiguration->DefaultAbsolutePantTiltPositionSpace->	c_str();
  LOG(INFO) << "Absolute Zoom Position Space: " 
    << configResponse->PTZConfiguration->DefaultAbsoluteZoomPositionSpace->	c_str();
  LOG(INFO) << "Relative Pan Tilt Translation Space: " 
    << configResponse->PTZConfiguration->DefaultRelativeZoomTranslationSpace->c_str();
  LOG(INFO) << "Relative Zoom Translation Space: " 
    << configResponse->PTZConfiguration->DefaultContinuousPanTiltVelocitySpace->c_str();
  LOG(INFO) << "Continuous Pan Tilt Velocity Space: " 
    << configResponse->PTZConfiguration->DefaultContinuousZoomVelocitySpace->c_str();
  LOG(INFO) << "Continuous Zoom Velocity Space: " 
    << configResponse->PTZConfiguration->DefaultContinuousZoomVelocitySpace->c_str();
  
  LOG(INFO) << "Default PTZ Timeout: " 
    << *configResponse->PTZConfiguration->DefaultPTZTimeout/1000;
  LOG(INFO) << "Pan Min Limit: " 
    << configResponse->PTZConfiguration->PanTiltLimits->Range->XRange->Min;
  LOG(INFO) << "Pan Max Limit: " 
    << configResponse->PTZConfiguration->PanTiltLimits->Range->XRange->Max;
  LOG(INFO) << "Tilt Min limit: " 
    << configResponse->PTZConfiguration->PanTiltLimits->Range->YRange->Min;
  LOG(INFO) << "Tilt Max limit: " 
    << configResponse->PTZConfiguration->PanTiltLimits->Range->YRange->Max;

  LOG(INFO) << "Coordinate System: " 
    << configResponse->PTZConfiguration->PanTiltLimits->Range->URI.c_str();

  LOG(INFO) << "Zoom Min limit: " 
    << configResponse->PTZConfiguration->ZoomLimits->Range->XRange->Min;
  LOG(INFO) << "Zoom Max limit: " 
    << configResponse->PTZConfiguration->ZoomLimits->Range->XRange->Max;
  LOG(INFO) << "Default Pan Speed: " 
    << configResponse->PTZConfiguration->DefaultPTZSpeed->PanTilt->x;
  LOG(INFO) << "Default Tilt Speed: " 
    << configResponse->PTZConfiguration->DefaultPTZSpeed->PanTilt->y;
  LOG(INFO) << "Default Zoom Speed: " 
    << configResponse->PTZConfiguration->DefaultPTZSpeed->Zoom->x;

  LOG(INFO) << "Coordinate System: " 
    << configResponse->PTZConfiguration->DefaultPTZSpeed->PanTilt->space->c_str();
  LOG(INFO) << "Coordinate System: " 
    << configResponse->PTZConfiguration->DefaultPTZSpeed->Zoom->space->c_str();

  soap_destroy(soap_); 
  soap_end(soap_);
}

void OnvifClientPTZ::panLeft(std::string profileToken, int nDegrees){
  float pan = -nDegrees*0.0027777778;
  return relativeMove(profileToken, pan, 1, 0.0, 0.0, 0.0, 0.0);
}

void OnvifClientPTZ::panRight(std::string profileToken, int nDegrees){
  float pan = nDegrees*0.0027777778;
  return relativeMove(profileToken, pan, 1, 0.0, 0.0, 0.0, 0.0);
}

void OnvifClientPTZ::tiltDown(std::string profileToken, int nDegrees){
  float tilt = -nDegrees*0.0055555556;
  return relativeMove(profileToken, 0.0, 0.0, tilt, 1.0, 0.0, 0.0);
}

void OnvifClientPTZ::tiltUp(std::string profileToken, int nDegrees){
  float tilt = nDegrees*0.0055555556;
  return relativeMove(profileToken, 0.0, 0.0, tilt, 1.0, 0.0, 0.0);
}


std::string OnvifClientPTZ::ErrorString() {
  std::string result = "";
  result += std::to_string(proxy_ptz_.soap->error);
  result += " FaultString : ";
  if (*soap_faultstring(proxy_ptz_.soap)) {
    std::string faultstring(*soap_faultstring(proxy_ptz_.soap));
    result += faultstring;
  } else {
    result += "null";
  }
  result += " FaultCode : ";
  if (*soap_faultcode(proxy_ptz_.soap)) {
    std::string faultcode(*soap_faultcode(proxy_ptz_.soap));
    result += faultcode;
  } else {
    result += "null";
  }
  result += " FaultSubcode : ";
  if (*soap_faultsubcode(proxy_ptz_.soap)) {
    std::string faultsubcode(*soap_faultsubcode(proxy_ptz_.soap));
    result += faultsubcode;
  } else {
    result += "null";
  }
  result += " FaultDetail : ";
  if (*soap_faultdetail(proxy_ptz_.soap)) {
    std::string faultdetail(*soap_faultdetail(proxy_ptz_.soap));
    result += faultdetail;
  } else {
    result += "null";
  }
  return result;
}
