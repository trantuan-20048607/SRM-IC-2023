#ifndef SRM_IC_2023_MODULES_CAMERA_HIK_CAMERA_HIK_H_
#define SRM_IC_2023_MODULES_CAMERA_HIK_CAMERA_HIK_H_

#include "camera-base/camera-base.h"

namespace camera::hik {
class HikCamera final : public Camera {
 public:
  HikCamera() = default;
  ~HikCamera() final = default;

  bool OpenCamera(const std::string &serial_number, const std::string &config_file) final;
  bool CloseCamera() final;
  bool StartStream() final;
  bool StopStream() final;
  bool GetFrame(Frame &frame) final;
  bool IsConnected() final;
  bool ExportConfigurationFile(const std::string &config_file) final;
  bool ImportConfigurationFile(const std::string &config_file) final;
  bool SetExposureTime(uint32_t exposure_time) final;
  bool SetGainValue(float gain) final;

 private:
  static void __stdcall ImageCallbackEx(unsigned char *image_data, MV_FRAME_OUT_INFO_EX *frame_info, void *obj);
  static void *DaemonThreadFunction(void *obj);
  bool SetExposureTimeHikImplementation(float exposure_time);

  void *device_{};
  static Registry<HikCamera> registry_;
};
}

#endif  // SRM_IC_2023_MODULES_CAMERA_HIK_CAMERA_HIK_H_
