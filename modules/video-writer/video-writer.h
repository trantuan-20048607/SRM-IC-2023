#ifndef SRM_IC_2023_MODULES_VIDEO_WRITER_VIDEO_WRITER_H_
#define SRM_IC_2023_MODULES_VIDEO_WRITER_VIDEO_WRITER_H_

#include <thread>
#include <atomic>
#include <opencv2/videoio.hpp>
#include "common/buffer.h"

namespace video_writer {
class VideoWriter final {
 public:
  VideoWriter() = default;
  ~VideoWriter();

  bool Open(const std::string &video_file, cv::Size frame_size, double fps = 60);
  void Write(cv::Mat &&frame);

 private:
  static void WritingThreadFunction(void *obj);

  std::unique_ptr<cv::VideoWriter> writer_;
  Buffer<cv::Mat, 512> buffer_;
  std::thread thread_;
  std::atomic_bool stop_flag_{};
};
}

#endif  // SRM_IC_2023_MODULES_VIDEO_WRITER_VIDEO_WRITER_H_
