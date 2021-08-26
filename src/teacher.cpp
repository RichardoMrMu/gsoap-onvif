#include "teacher.hpp"
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>


using namespace cv;
using namespace std;
using namespace wl;


void TeaAnalyser::SetReader(const wl::CameraConfig& config
)

{
  LOG(INFO) << "Test HIKIPCameraReader.";
  std::string url = "rtsp://" + config.username + ":" + config.password + "@"
    + config.ip + "/mpeg4/ch1/sub/av_stream";
  reader_.reset(new OpenCVCameraReader(url));
  ptzer_.reset(new Ptz(config));
}



void TeaAnalyser::Run() {
  fs::Timer timer;
  //std::vector<int> PresetTokens = {1,2};
  while (!cam_is_stop_) {
    timer.tic();
	for(auto &PresetToken:PresetTokens){
        // goto one preset
		ptzer_->GotoPreset(PresetToken);
		
		wait(1300);
		// get one frame 
		reader_->Next(&frame_);
		//save one frame 

		std::string frame_path = "/blackboard.jpg";
		
		cv::imwrite(frame_path, frame_.img);
		
		timer.toc();
		LOG(INFO) << "frame : " << frame_.frame_id << " cost: " << timer.total_time();
  }
}
	
}

