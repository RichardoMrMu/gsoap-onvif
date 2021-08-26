#include "reader.hpp"
#include <ncurses.h>
#include <string>
#include <iostream>

namespace wl {

static void wait(int t) {
  std::this_thread::sleep_for(std::chrono::milliseconds(t));
}

OpenCVCameraReader::OpenCVCameraReader(const std::string uri) 
  : uri_(uri), counter_(0), ch_(2) {
  worker_ = std::thread([this] {
    cap_.open(uri_);
    CHECK(cap_.isOpened()) << "Can't open " << uri_;
    for (;;) {
      cv::Mat img;
      if (cap_.read(img)) {
        Frame frame;
        frame.frame_id = counter_;
        cv::resize(img, frame.img, cv::Size(1024, 576), 0, 0);
        counter_++;
        CHECK(ch_.Push(std::move(frame)));
      } else {
        LOG(ERROR) << "No frame";
        break;
      }
    }
    cap_.release();
    ch_.Close();
  });
}

OpenCVCameraReader::~OpenCVCameraReader() {
  worker_.join();
  CHECK_EQ(ch_.IsClosed(), true) << "Reader hasn\'t finished.";
}

bool OpenCVCameraReader::Next(Frame* frame) {  
  return ch_.Receive(frame);
}

Ptz::Ptz(const CameraConfig& config)
  : ip_(config.ip), name_(config.username), passwd_(config.password),
    zoom_(false), speed_(0.0), zoom_scale_(0.0) {
  ptz_.reset(new OnvifClientPTZ(ip_, name_, passwd_, true));
  media_.reset(new OnvifClientMedia(ip_, name_, passwd_, true));
	// get media profile
	media_->getProfiles(profiles_);
	for (int i = 0; i < profiles_.size(); ++i) {
		LOG(INFO) << i <<" "<<profiles_[i].profileName << "  " << profiles_[i].profileToken ;
			
}

}

Ptz::~Ptz() {
  Reset();
}

void Ptz::GotoPreset(int PresetToken){
	ptz_->gotoPreset(profiles_[0].profileToken,PresetToken,0.5);
}

// if speed > 0, move right; else move left
void Ptz::Move(float speed) {
  CHECK_LE(speed, 1.0) << "speed can't be greater than 1.0";
  CHECK_GE(speed, -1.0) << "speed can't be smaller than -1.0";
  if (speed_ - speed < 0.0001 && speed_ - speed > -0.0001) {
    return;
  }
  speed_ = speed;
  if (speed - 0.0 < 0.0001 && speed - 0.0 > -0.0001) {
    // stop move when speed->0
    ptz_->stop(profiles_[0].profileToken, 0, 0);
  } else {
    if (speed > 0) {
      LOG(INFO) << "Move right";
    } else {
      LOG(INFO) << "Move left";
    }
    ptz_->continuousMove(profiles_[0].profileToken, speed, 0, 0);
  }
}

void Ptz::ZoomIn(float scale) {
  if (!zoom_) {
    CHECK_LE(scale, 1.0) << "scale can't be greater than 1.0";
    CHECK_GE(scale, -1.0) << "scale can't be smaller than -1.0";
    zoom_scale_ = scale;
    zoom_ = true;
    ptz_->relativeMove(profiles_[0].profileToken, 0.0, 0.0, 0.0, 0.0, scale, 1);
    wait(500);
  }
}

void Ptz::ZoomOut() {
  if (!zoom_) {
    CHECK_LE(zoom_scale_, 1.0) << "scale can't be greater than 1.0";
    CHECK_GE(zoom_scale_, -1.0) << "scale can't be smaller than -1.0";
    CHECK(zoom_) << "can't zoom out without zoom in";
    zoom_ = false;
    float scale = 0 - zoom_scale_;
    ptz_->relativeMove(profiles_[0].profileToken, 0.0, 0.0, 0.0, 0.0, scale, 1);
    wait(500);
  }
}

void Ptz::Reset() {
  ZoomOut();
  ptz_->goToHomePosition(profiles_[0].profileToken);
  wait(800);
}

OpenCVVideoReader::OpenCVVideoReader(const std::string uri)
  : counter_(0) {
  cap_.open(uri);
  CHECK(cap_.isOpened()) << "Can\'t open " << uri;
}

OpenCVVideoReader::~OpenCVVideoReader() {
  cap_.release();
}

bool OpenCVVideoReader::Next(Frame* frame) {
  cv::Mat img;
  if (!cap_.read(img)) {
    return false;
  }
  else {
    frame->frame_id = counter_;
    frame->img = img;
    counter_++;
    return true;
  }
}

OpenCVImageReader::OpenCVImageReader(const std::vector<std::string>& img_lst)
  : img_lst_(img_lst), counter_(0) {}

bool OpenCVImageReader::Next(Frame* frame) {
  if (counter_ < img_lst_.size()) {
    cv::Mat img = cv::imread(img_lst_[counter_]);
    CHECK(img.data) << "Can\'t read " << img_lst_[counter_];
    frame->frame_id = counter_;
    frame->img = img;
    counter_++;
    return true;
  }
  else {
    return false;
  }
}

}  // namespace wl
