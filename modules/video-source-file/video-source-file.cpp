#include <glog/logging.h>
#include "video-source-file.h"

video_source::Registry<video_source::file::FileVideoSource>
    video_source::file::FileVideoSource::registry_("file");

video_source::file::FileVideoSource::~FileVideoSource() {
  if (video_.isOpened()) video_.release();
}

bool video_source::file::FileVideoSource::Initialize(const std::string &config_file) {
  cv::FileStorage video_init_config;
  video_init_config.open(config_file, cv::FileStorage::READ);
  if (!video_init_config.isOpened()) {
    LOG(ERROR) << "Failed to open camera initialization file " << config_file << ".";
    return false;
  }
  std::string all_cams_config_file;
  video_init_config["ALL_CAMS_CONFIG_FILE"] >> all_cams_config_file;
  if (all_cams_config_file.empty()) {
    LOG(ERROR) << "All cameras' config file configuration not found.";
    return false;
  }
  cv::FileStorage all_cams_config;
  all_cams_config.open(all_cams_config_file, cv::FileStorage::READ);
  if (!all_cams_config.isOpened()) {
    LOG(ERROR) << "Failed to open all cameras' config file " << all_cams_config_file << ".";
    return false;
  }
  std::string all_lens_config_file;
  video_init_config["ALL_LENS_CONFIG_FILE"] >> all_lens_config_file;
  if (all_lens_config_file.empty()) {
    LOG(ERROR) << "All lens' config file configuration not found.";
    return false;
  }
  cv::FileStorage all_lens_config;
  all_lens_config.open(all_lens_config_file, cv::FileStorage::READ);
  if (!all_lens_config.isOpened()) {
    LOG(ERROR) << "Failed to open all lens' config file " << all_lens_config_file << ".";
    return false;
  }
  std::string len_type;
  all_cams_config[video_init_config["CAMERA"]]["LEN"] >> len_type;
  all_lens_config[len_type]["IntrinsicMatrix"] >> intrinsic_mat_;
  all_lens_config[len_type]["DistortionMatrix"] >> distortion_mat_;
  if (intrinsic_mat_.empty() || distortion_mat_.empty()) {
    LOG(ERROR) << "Camera len configurations not found.";
    intrinsic_mat_.release();
    distortion_mat_.release();
    return false;
  }
  std::string video_file;
  video_init_config["VIDEO"] >> video_file;
  if (video_file.empty()) {
    LOG(ERROR) << "Video configuration not found.";
    intrinsic_mat_.release();
    distortion_mat_.release();
    return false;
  }
  video_.open(video_file);
  if (!video_.isOpened()) {
    LOG(ERROR) << "Failed to open video file " << video_file << ".";
    intrinsic_mat_.release();
    distortion_mat_.release();
    return false;
  }
  return true;
}

bool video_source::file::FileVideoSource::GetFrame(Frame &frame) {
  if (video_.read(frame.image)) {
    time_stamp_ += uint64_t(1e9 / video_.get(cv::CAP_PROP_FPS));
    frame.time_stamp = time_stamp_;
    for (auto p : callback_list_)
      if (p.first) (*p.first)(p.second, frame);
    return true;
  } else
    return false;
}

void video_source::file::FileVideoSource::RegisterFrameCallback(FrameCallback callback, void *obj) {
  callback_list_.emplace_back(callback, obj);
}

void video_source::file::FileVideoSource::UnregisterFrameCallback(FrameCallback callback) {
  callback_list_.erase(std::remove_if(
      callback_list_.begin(), callback_list_.end(),
      [callback](auto p) { return p.first == callback; }), callback_list_.end());
}
