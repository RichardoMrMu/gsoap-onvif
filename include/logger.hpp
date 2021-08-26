#include <string>
#include "glog/logging.h"

class LoggerWorker {
public:
  LoggerWorker() {};
  ~LoggerWorker() {
    google::ShutdownGoogleLogging();
  };
  void Init(std::string logname, bool write) {
    google::InitGoogleLogging(logname.c_str());
    google::InstallFailureSignalHandler();
    FLAGS_colorlogtostderr = true;
    FLAGS_logbufsecs = 0;
    FLAGS_max_log_size = 10; // file size(MB)
    FLAGS_logtostderr = write; //write logs when it's false
  }
};