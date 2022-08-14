#ifndef SRM_IC_2023_MODULES_VIDEO_WRITER_VIDEO_WRITER_H_
#define SRM_IC_2023_MODULES_VIDEO_WRITER_VIDEO_WRITER_H_

#include <opencv2/videoio.hpp>

namespace video_writer {
class VideoWriter final {
 public:
  VideoWriter() = default;
  ~VideoWriter() = default;

  bool Open(const std::string &video_file, cv::Size frame_size, double fps = 60);
  void Write(const cv::Mat &frame);

 private:
  std::unique_ptr<cv::VideoWriter> writer_;
};
}

#endif  // SRM_IC_2023_MODULES_VIDEO_WRITER_VIDEO_WRITER_H_
