#include "cli-arg-parser/cli-arg-parser.h"
#include "controller-base.h"

bool controller::Controller::Initialize(const std::string &type_name) {
  video_source_.reset(video_source::CreateVideoSource(cli_argv.VideoSourceType()));
  if (!video_source_) {
    LOG(ERROR) << "Failed to create type " << type_name << " video source.";
    return false;
  }
  if (!video_source_->Initialize("../config/" + type_name + "/" + cli_argv.VideoSourceType() + "-init.yaml")) {
    LOG(ERROR) << "Failed to initialize video source.";
    video_source_.reset();
    return false;
  }
  if (cli_argv.Serial()) {
    serial_ = std::make_unique<serial::Serial>();
    if (!serial_->Open()) {
      serial_->Close();
      return false;
    }
  }
  video_source_->RegisterFrameCallback(&FrameCallback, this);
  // TODO INIT MATRIX
  return true;
}

std::function<void(void *obj, Frame &)> controller::Controller::FrameCallback = [](void *obj, Frame &frame) {
  auto self = (controller::Controller *) obj;
  if (self->serial_ && !self->serial_->ReadData(frame.receive_packet))
    LOG(WARNING) << "Failed to read data from serial port in frame callback function.";
};
