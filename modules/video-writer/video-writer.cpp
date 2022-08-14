#include <glog/logging.h>
#include "video-writer.h"

void video_writer::VideoWriter::Write(const cv::Mat &frame) {
  if (writer_) writer_->write(frame);
}

bool video_writer::VideoWriter::Open(const std::string &video_file, cv::Size frame_size, double fps) {
  writer_ = std::make_unique<cv::VideoWriter>();
  if (!writer_->open(video_file, cv::CAP_FFMPEG, cv::VideoWriter::fourcc('a', 'v', 'c', '1'), fps, frame_size)) {
    LOG(INFO) << "Failed to open video file " << video_file << ".";
    writer_.reset();
    return false;
  }
  LOG(INFO) << "Opened video file " << video_file << ".";
  return true;
}
