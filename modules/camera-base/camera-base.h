#ifndef SRM_IC_2023_MODULES_CAMERA_BASE_CAMERA_BASE_H_
#define SRM_IC_2023_MODULES_CAMERA_BASE_CAMERA_BASE_H_

#include <atomic>
#include "common/factory.h"
#include "common/frame.h"
#include "common/buffer.h"

enable_factory(camera, Camera)

namespace camera {
class Camera {
 public:
  Camera() = default;
  virtual ~Camera() = default;

  virtual bool OpenCamera(const std::string &serial_number, const std::string &config_file) = 0;
  virtual bool CloseCamera() = 0;
  virtual bool GetFrame(Frame &frame) = 0;
  virtual bool StartStream() = 0;
  virtual bool StopStream() = 0;
  virtual bool ImportConfigurationFile(const std::string &config_file) = 0;
  virtual bool ExportConfigurationFile(const std::string &config_file) = 0;
  virtual bool IsConnected() = 0;
  virtual bool SetExposureTime(uint32_t exposure_time) = 0;
  virtual bool SetGainValue(float gain) = 0;

  void RegisterFrameCallback(FrameCallback callback, void *obj);
  void UnregisterFrameCallback(FrameCallback callback);

 protected:
  std::vector<std::pair<FrameCallback, void *>> callback_list_;
  std::string serial_number_;
  bool stream_running_{};
  std::atomic_bool stop_flag_{};
  pthread_t daemon_thread_id_{};
  Buffer<Frame, 2> buffer_;
};
}

#endif  // SRM_IC_2023_MODULES_CAMERA_BASE_CAMERA_BASE_H_
