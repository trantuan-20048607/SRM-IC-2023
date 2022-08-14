#ifndef SRM_IC_2023_MODULES_VIDEO_SOURCE_FILE_VIDEO_SOURCE_FILE_H_
#define SRM_IC_2023_MODULES_VIDEO_SOURCE_FILE_VIDEO_SOURCE_FILE_H_

#include <opencv2/videoio.hpp>
#include "video-source-base/video-source-base.h"

namespace video_source::file {
class FileVideoSource final : public VideoSource {
 public:
  FileVideoSource() = default;
  ~FileVideoSource() final = default;

  bool Initialize(const std::string &config_file) final;
  bool GetFrame(Frame &frame) final;
  void RegisterFrameCallback(FrameCallback callback, void *obj) final;
  void UnregisterFrameCallback(FrameCallback callback) final;

 private:
  double frame_rate_{};
  std::vector<std::pair<FrameCallback, void *>> callback_list_;
  cv::VideoCapture video_;
  uint64_t time_stamp_{};
  static Registry<FileVideoSource> registry_;
};
}

#endif  // SRM_IC_2023_MODULES_VIDEO_SOURCE_FILE_VIDEO_SOURCE_FILE_H_
