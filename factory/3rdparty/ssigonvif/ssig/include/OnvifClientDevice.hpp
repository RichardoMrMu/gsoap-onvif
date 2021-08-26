#ifndef ONVIFCLIENTDEVICE_HPP
#define ONVIFCLIENTDEVICE_HPP

#include <iostream>
#include "stdio.h"
#include "gsoap/wsseapi.h"
#include <openssl/rsa.h>
 
#include "onvif/soapDeviceBindingProxy.h"

#include<stdio.h>
#include<string.h>
#include<time.h>

#define MAX_MSG_LEN 1024

typedef unsigned int uint;

class OnvifClientDevice{
public:
  OnvifClientDevice(std::string url,
                    std::string user,
                    std::string password,
                    bool showCapabilities);
  ~OnvifClientDevice();
  virtual std::string ErrorString();

protected:
  bool has_media_;
  bool has_ptz_;
  std::string device_url_;
  std::string media_url_;
  std::string ptz_url_;
  std::string user_;
  std::string passwd_;
  struct soap *soap_;
private:
  DeviceBindingProxy proxy_device_;
};

#endif








