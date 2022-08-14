#ifndef SRM_IC_2023_MODULES_CONTROLLER_BASE_CONTROLLER_BASE_H_
#define SRM_IC_2023_MODULES_CONTROLLER_BASE_CONTROLLER_BASE_H_

#include <memory>
#include "serial/serial.h"
#include "video-source-base/video-source-base.h"
#include "video-writer/video-writer.h"

void SignalHandler(int signal);
enable_factory(controller, Controller)

namespace controller {
class Controller {
  friend void::SignalHandler(int signal);
 public:
  Controller() = default;
  virtual ~Controller() = default;

  virtual bool Initialize() = 0;
  virtual int Run() = 0;

 protected:
  bool Initialize(const std::string &type_name);

  static bool exit_signal_;

  std::unique_ptr<video_source::VideoSource> video_source_;
  std::unique_ptr<serial::Serial> serial_;
  SendPacket send_packet_;
  ReceivePacket receive_packet_;
  Frame frame_;
  video_writer::VideoWriter video_writer_;

 private:
  static std::function<void(void *, Frame &)> FrameCallback;
};
}

#endif  // SRM_IC_2023_MODULES_CONTROLLER_BASE_CONTROLLER_BASE_H_
