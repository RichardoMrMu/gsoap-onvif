#pragma once
#include <mutex>
#include <vector>
#include <string>
#include <thread>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include "OnvifClientDevice.hpp"
#include "OnvifClientPTZ.hpp"
#include "OnvifClientMedia.hpp"
#include "channel.hpp"


namespace wl {

struct Frame {
  cv::Mat img;
  int frame_id;
};

struct CameraConfig {
  std::string ip;
  int port;
  std::string username;
  std::string password;
};

class Reader {
public:
  virtual ~Reader() {}
  virtual bool Open() { return true; }
  virtual bool Next(Frame* frame) = 0;
  virtual bool Close() { return true; }
};

class Ptz {
public:
  Ptz(const CameraConfig& config);
  ~Ptz();
  void Move(float speed);
  void ZoomIn(float scale);
  void ZoomOut();
  void Reset();
	void GotoPreset(int PresetToken);
private:
  std::string ip_;
  std::string name_;
  std::string passwd_;
  std::shared_ptr<OnvifClientPTZ> ptz_;
  std::shared_ptr<OnvifClientMedia> media_;
	std::vector<_ocp_Profile> profiles_;
  bool zoom_;
  float zoom_scale_;
  float speed_;
};

class OpenCVCameraReader : public Reader {
public:
  OpenCVCameraReader(const std::string uri);
  ~OpenCVCameraReader();

  virtual bool Next(Frame* frame) override;
private:
  std::string uri_;
  int counter_;
  cv::VideoCapture cap_;
  std::thread worker_;
  Channel<Frame> ch_;
};

class OpenCVVideoReader : public Reader {
public:
  OpenCVVideoReader(const std::string uri);
  ~OpenCVVideoReader();
  virtual bool Next(Frame* frame) override;

private:
  std::string uri_;
  int counter_;
  cv::VideoCapture cap_;
};

class OpenCVImageReader : public Reader {
public:
  OpenCVImageReader(const std::vector<std::string>& img_lst);

  virtual bool Next(Frame* frame) override;

private:
  int counter_;
  std::vector<std::string> img_lst_;
};

}  // namespace wl
