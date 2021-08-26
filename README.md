# Gsoap-Onvif
This repository used gsoap onvif to get images, go to presets and move. It can run in jetson nano 2g version and test on the hikCamera. This repository do the secondary wrap for [ssig-onvif](https://github.com/smartsenselab/ssig-onvif), and give a specific  instruction to use it.
<details>
<summary>Step1. Install Ssig-Onvif</summary>


1. Install opnessl

```shell
# download openssl from web 
wget https://www.openssl.org/source/old/1.0.2/openssl-1.0.2l.tar.gz
tar -xzvf openssl-1.0.2l.tar.gz
cd openssl-1.0.2l
sudo ./config
sudo make 
sudo make test # you can test it with this line 
sudo make install 
# check openssl version
openssl version
```

2. Install libssl1.0

```shell
sudo apt-get install libssl1.0-dev
```

</details>

<details>
<summary>Step2. Build Project</summary>
Next step is build the project, you should remove the files in build fold,and the rebuild the project by the following lines.

```shell
cd gsoap-onvif
cd build 
rm -rf *
cmake ..
make -j 
./zoom_tea
```



</details>

## Deployment
From now, gsoap-onvif achieve the functions about absolute move, relative move, goto preset, get preset, get ptz configuration, get meida configuration, get device configuration, stop, zoom in and zoom out. You can use it control the camera and take photos with opencv.
The specific test code is in [here](https://github.com/RichardoMrMu/gsoap-onvif/blob/main/factory/test/test_analyser.cpp).
### Init
You can use the following codes to init the PTZ and Media, to control the camera use ptz and get camera settings by media.

```c++
#include <chrono>
#include <iostream>
#include <sstream> 
#include "glog/logging.h"
#include "OnvifClientDevice.hpp"
#include "OnvifClientPTZ.hpp"
#include "OnvifClientMedia.hpp"
std::string url = "192.168.66.64";
std::string name = "admin";
std::string password = "wst123456";
OnvifClientPTZ *PTZ = new OnvifClientPTZ(url, name, password, true);
OnvifClientMedia *Media = new OnvifClientMedia(url, name, password, true);
```

### Get configuration
You can get the Meida configuration

```c++
std::vector<_ocp_Profile> get_profilesResponse;
Media->getProfiles(get_profilesResponse);
```

### Goto preset
gotoPreset : the first parm can get from Media->getProfiles.
The secondd parm is your setting preset like 1,2,3, type as int.
The third parm is speed, between 0 to 1.0, type as float.

```c++
PTZ->gotoPreset(get_profilesResponse[0].profileToken,3,0.5);
```

### Absolute move and Relative move
You can control the camera to move.

```c++
PTZ->relativeMove(get_profilesResponse[0], 0.0, 0.0, 0.0, 0.0, 0.5, 1);
PTZ->absoluteMove(get_profilesResponse[0], 0.0, 0.0, 0.0, 0.0, 0.5, 1);
```

