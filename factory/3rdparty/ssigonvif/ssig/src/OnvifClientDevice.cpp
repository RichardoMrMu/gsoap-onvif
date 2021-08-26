#include "OnvifClientDevice.hpp"
#include <iostream> 
#include <sstream> 
#include "glog/logging.h"
#include "DeviceBinding.nsmap"

OnvifClientDevice::OnvifClientDevice(std::string url,
                                     std::string user,
                                     std::string password,
                                     bool showCapabilities) {
  device_url_ = "http://" + url + "/onvif/device_service";
  user_ = user;
  passwd_ = password;

  has_media_ = false;
  has_ptz_ = false;

  proxy_device_.soap_endpoint = device_url_.c_str();
  
  soap_register_plugin(proxy_device_.soap, soap_wsse);
  soap_ = soap_new();

  if (SOAP_OK != soap_wsse_add_UsernameTokenDigest(proxy_device_.soap,
                                                   NULL, 
                                                   user.c_str(),
                                                   password.c_str())) {
    LOG(INFO) << "Device binding 1 Error";
  }

  if (SOAP_OK != soap_wsse_add_Timestamp(proxy_device_.soap, "Time", 10))
  {
    LOG(INFO) << "Device binding 2 Error";
  }

  auto *get_cap = soap_new__tds__GetCapabilities(soap_, -1);
  get_cap->Category.push_back(tt__CapabilityCategory__All);
  auto *response = soap_new__tds__GetCapabilitiesResponse(soap_, -1);

  if (SOAP_OK == proxy_device_.GetCapabilities(get_cap, response)) {
  
    if (response->Capabilities->Media != NULL) {
      has_media_ = true;

      if(showCapabilities) {
        LOG(INFO) << "--------------------------Media-----------------------";
        LOG(INFO) << "XAddr : " << response->Capabilities->Media->XAddr;
      }
      media_url_ = response->Capabilities->Media->XAddr;
    }

    if (response->Capabilities->PTZ != NULL) {
      has_ptz_ = true;
      if(showCapabilities) {
        LOG(INFO) << "--------------------------PTZ-------------------------";
        LOG(INFO) << "XAddr : " << response->Capabilities->PTZ->XAddr;
      }
      ptz_url_ = response->Capabilities->PTZ->XAddr;
    }
  }
  else {
    LOG(FATAL) << "Error : " << ErrorString();
  }
  soap_destroy(soap_); 
  soap_end(soap_);
}

OnvifClientDevice::~OnvifClientDevice(){

}

std::string OnvifClientDevice::ErrorString() {
  std::string result = "";
  result += std::to_string(proxy_device_.soap->error);
  result += " FaultString : ";
  if (*soap_faultstring(proxy_device_.soap)) {
    std::string faultstring(*soap_faultstring(proxy_device_.soap));
    result += faultstring;
  } else {
    result += "null";
  }
  result += " FaultCode : ";
  if (*soap_faultcode(proxy_device_.soap)) {
    std::string faultcode(*soap_faultcode(proxy_device_.soap));
    result += faultcode;
  } else {
    result += "null";
  }
  result += " FaultSubcode : ";
  if (*soap_faultsubcode(proxy_device_.soap)) {
    std::string faultsubcode(*soap_faultsubcode(proxy_device_.soap));
    result += faultsubcode;
  } else {
    result += "null";
  }
  result += " FaultDetail : ";
  if (*soap_faultdetail(proxy_device_.soap)) {
    std::string faultdetail(*soap_faultdetail(proxy_device_.soap));
    result += faultdetail;
  } else {
    result += "null";
  }
  return result;
}

