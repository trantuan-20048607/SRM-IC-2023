#include <opencv2/opencv.hpp>
#include "controller-hero.h"

controller::Registry<controller::hero::HeroController> controller::hero::HeroController::registry_("hero");

bool controller::hero::HeroController::Initialize() {
  return controller::Controller::Initialize("hero");
}

int controller::hero::HeroController::Run() {
  while (!video_source_->GetFrame(frame_)) {
    LOG(WARNING) << "Failed to get frame data from video source."
                 << " Wait for reconnecting the camera or press Ctrl-C to exit.";
    sleep(1);
  }
  auto start_time = frame_.time_stamp;
  bool pause = false;
  while (!exit_signal_) {
    if (!pause) {
      if (!video_source_->GetFrame(frame_)) {
        LOG(WARNING) << "Failed to get frame data from video source."
                     << " Wait for reconnecting the camera or press Ctrl-C to exit.";
      } else {
        std::ostringstream ss_time;
        ss_time << "TIME: " << std::fixed << std::setprecision(1)
                << 1e-9 * static_cast<double>(frame_.time_stamp - start_time) << "s";
        std::string str_time = ss_time.str();
        cv::putText(frame_.image, str_time, cv::Point(0, 25), cv::FONT_HERSHEY_DUPLEX, 1, cv::Scalar(0, 255, 0));
        cv::imshow("HERO", frame_.image);
      }
    }
    auto key = cv::waitKey(1);
    if (key == 'q')
      break;
    else if (key == 'p')
      pause = !pause;
  }
  cv::destroyAllWindows();
  return 0;
}
