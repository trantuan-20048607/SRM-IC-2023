#include <glog/logging.h>
#include "video-writer.h"

video_writer::VideoWriter::~VideoWriter() {
  stop_flag_ = true;
  if (thread_.joinable()) thread_.join();
  writer_.reset();
}

bool video_writer::VideoWriter::Open(const std::string &video_file, cv::Size frame_size, double fps) {
  writer_ = std::make_unique<cv::VideoWriter>();
  if (!writer_->open(video_file, cv::CAP_FFMPEG, cv::VideoWriter::fourcc('a', 'v', 'c', '1'), fps, frame_size)) {
    LOG(INFO) << "Failed to open video file " << video_file << ".";
    writer_.reset();
    return false;
  }
  thread_ = std::thread(WritingThreadFunction, this);
  LOG(INFO) << "Opened video file " << video_file << ".";
  return true;
}

void video_writer::VideoWriter::Write(cv::Mat &&frame) {
  if (writer_) buffer_.Push(std::move(frame));
}

void video_writer::VideoWriter::WritingThreadFunction(void *obj) {
  auto self = static_cast<VideoWriter *>(obj);
  while (!self->stop_flag_) {
    cv::Mat frame;
    if (self->buffer_.Pop(frame))
      self->writer_->write(frame);
  }
}
