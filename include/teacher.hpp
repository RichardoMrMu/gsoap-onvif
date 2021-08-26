#pragma once
#include <thread>
#include <string>
#include <fstream>
#include "workloop.hpp"

class StableLocation {
public:
  int x_ = -1;
  int y_ = -1;
  int frame_num_ = -1;
};

class TeaAnalyser {
public:
  TeaAnalyser() {
    //Initialize(gpu);
  };
  void SetReader(const wl::CameraConfig& config,std::unique_ptr<HttpLink> & httplink);
  void Run();
	std::string ObjectNameProduce(bool realtime);

private:
  std::unique_ptr<wl::OpenCVCameraReader> reader_;
  std::unique_ptr<wl::Ptz> ptzer_;
  wl::Frame frame_;
  //cv::Rect bbox_new_;
  // 最近一次有人的帧数
  int last_person_frame_ = -1;
  // 最近一次举手的帧数
  int raising_frame_ = -1;
  // 稳定的位置
  StableLocation last_stable_location_;
  // 是否拉近镜头, 针对人站立不动的情况
  bool zoom_in_ = false;
  // 是否拉远镜头
  bool zoom_out_ = false;
	// whether camera threading is on
	bool cam_is_stop_=false;
	// upload file oss link
};
