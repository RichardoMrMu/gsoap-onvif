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
  // ���һ�����˵�֡��
  int last_person_frame_ = -1;
  // ���һ�ξ��ֵ�֡��
  int raising_frame_ = -1;
  // �ȶ���λ��
  StableLocation last_stable_location_;
  // �Ƿ�������ͷ, �����վ�����������
  bool zoom_in_ = false;
  // �Ƿ���Զ��ͷ
  bool zoom_out_ = false;
	// whether camera threading is on
	bool cam_is_stop_=false;
	// upload file oss link
};
