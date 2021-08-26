#include <chrono>
#include <memory>
#ifdef _MSC_VER
  #include <windows.h>
#endif 
#include "teacher.hpp"
#include "fs.hpp"
#include "logger.hpp"

using namespace std;

int main(int argc, char** argv) {
  TeaAnalyser teacher(0);
  wl::CameraConfig config;
  config.ip = "192.168.66.64";
  config.port = 8000;
  config.username = "admin";
  config.password = "wst123456";
  teacher.SetReader(config);
  teacher.Run();
  return 0;
}

