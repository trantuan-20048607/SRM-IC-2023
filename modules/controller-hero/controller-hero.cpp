#include <opencv2/opencv.hpp>
#include "controller-hero.h"
#include "cli-arg-parser/cli-arg-parser.h"

controller::Registry<controller::hero::HeroController> controller::hero::HeroController::registry_("hero");

bool controller::hero::HeroController::Initialize() {
  return controller::Controller::Initialize("hero");
}

int controller::hero::HeroController::Run() {
  double fps = 0, show_fps = 0;
  bool pause = false, show_warning = true;
  struct timespec ts_start{};

  constexpr auto frame_time_str = [](auto time_stamp) {
    static auto start_time = time_stamp;
    auto delta_time_ns = time_stamp - start_time;
    double delta_time_s = static_cast<double>(delta_time_ns) * 1e-9;
    auto delta_time_m = static_cast<uint32_t>(std::floor(delta_time_s / 60));
    delta_time_s -= delta_time_m * 60;
    auto delta_time_h = delta_time_m / 60;
    delta_time_m -= delta_time_h * 60;
    std::ostringstream ss_time;
    if (delta_time_h) ss_time << std::setw(2) << std::setfill('0') << delta_time_h << ":";
    ss_time << std::setw(2) << std::setfill('0') << delta_time_m << ":"
            << std::setw(5) << std::setfill('0') << std::fixed << std::setprecision(2)
            << delta_time_s;
    return ss_time.str();
  };

  auto update_frame_data = [&]() {
    if (pause) return false;
    auto ret = video_source_->GetFrame(frame_);
    if (!ret && show_warning)
      LOG(WARNING) << "Failed to get frame data from video source."
                   << " Wait for reconnecting the camera or press Ctrl-C to exit.";
    show_warning = ret;
    return ret;
  };

  auto update_window = [&](const std::string &title) {
    static uint32_t rec_frame_count = 0;
    std::ostringstream ss_fps;
    ss_fps << std::fixed << std::setprecision(0) << show_fps;
    if (!pause && show_warning) {
      if (cli_argv.Record()) {
        video_writer_.Write(frame_.image);
        ++rec_frame_count;
      }
      if (cli_argv.UI()) {
        cv::putText(frame_.image, frame_time_str(frame_.time_stamp),
                    cv::Point(0, 24), cv::FONT_HERSHEY_SIMPLEX, 1, cv::Scalar(0, 192, 0));
        cv::putText(frame_.image, "FPS: " + ss_fps.str(),
                    cv::Point(0, 48), cv::FONT_HERSHEY_SIMPLEX, 1, cv::Scalar(0, 192, 0));
        if (cli_argv.Record())
          cv::putText(frame_.image, "REC: " + std::to_string(rec_frame_count), cv::Point(0, 72),
                      cv::FONT_HERSHEY_SIMPLEX, 1, cv::Scalar(0, 0, 192));
        cv::imshow(title, frame_.image);
      }
    }
  };

  auto start_count_fps = [&]() {
    clock_gettime(CLOCK_REALTIME, &ts_start);
  };

  auto stop_count_fps = [&]() {
    struct timespec ts_end{};
    clock_gettime(CLOCK_REALTIME, &ts_end);
    auto delta_time_ns = ts_end.tv_nsec - ts_start.tv_nsec;
    if (delta_time_ns > 0)
      fps = 1e9 / static_cast<double>(delta_time_ns);
  };

  auto check_key = [&]() {
    if (cli_argv.UI()) {
      auto key = cv::waitKey(1);
      if (key == 'q') {
        LOG(INFO) << "QUIT";
        exit_signal_ = true;
      } else if (key == 'p') {
        LOG(INFO) << (pause ? "RESUME" : "PAUSE");
        pause = !pause;
      }
    }
  };

  std::thread auto_log_fps([&]() {
    while (!exit_signal_) {
      if (!pause && show_warning) {
        show_fps = fps;
        LOG(INFO) << "FPS: " << fps;
      }
      sleep(1);
    }
  });

  while (!exit_signal_) {
    start_count_fps();
    update_frame_data();
    update_window("HERO");
    stop_count_fps();
    check_key();
  }

  auto_log_fps.join();
  cv::destroyAllWindows();
  return 0;
}
