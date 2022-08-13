#ifndef SRM_IC_2023_MODULES_CONTROLLER_BASE_CONTROLLER_BASE_H_
#define SRM_IC_2023_MODULES_CONTROLLER_BASE_CONTROLLER_BASE_H_

#include <memory>
#include "serial/serial.h"
#include "video-source-base/video-source-base.h"

void SignalHandler(int signal);

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
  SendPacket send_packet_;
  ReceivePacket receive_packet_;
  std::unique_ptr<serial::Serial> serial_;
  Frame frame_;
 private:
  static std::function<void(void *, Frame &)> FrameCallback;
};

using Factory = factory::Factory<Controller>;
template<class T> using Registry = factory::RegistrySub<Controller, T>;
Controller *CreateController(const std::string &type_name);
}

#endif  // SRM_IC_2023_MODULES_CONTROLLER_BASE_CONTROLLER_BASE_H_
