#ifndef SRM_IC_2023_MODULES_CAMERA_DH_CAMERA_DH_H_
#define SRM_IC_2023_MODULES_CAMERA_DH_CAMERA_DH_H_

#include "camera-base/camera-base.h"

namespace camera::dh {
class DHCamera final : public Camera {
 public:
  DHCamera() = default;
  ~DHCamera() final;

  bool OpenCamera(const std::string &serial_number, const std::string &config_file) final;
  bool CloseCamera() final;
  bool StartStream() final;
  bool StopStream() final;
  bool IsConnected() final;
  bool GetFrame(Frame &frame) final;
  bool ExportConfigurationFile(const std::string &config_file) final;
  bool ImportConfigurationFile(const std::string &config_file) final;
  bool SetExposureTime(uint32_t exposure_time) final;
  bool SetGainValue(float gain_value) final;

 private:
  bool RegisterCaptureCallback(GXCaptureCallBack callback);
  bool UnregisterCaptureCallback();
  bool SetExposureTimeDHImplementation(int64_t exposure_time);
  bool SetExposureMode(GX_EXPOSURE_MODE_ENTRY gx_exposure_mode_entry);
  bool SetExposureTimeMode(GX_EXPOSURE_TIME_MODE_ENTRY gx_exposure_time_mode_entry);
  bool SetGainValueDHImplementation(double gain);
  bool SetGainAuto(GX_GAIN_AUTO_ENTRY gx_gain_auto_entry);
  static std::string GetErrorInfo(GX_STATUS error_status_code);
  bool Raw8Raw16ToRGB24(GX_FRAME_CALLBACK_PARAM *frame_callback);
  static void GX_STDC DefaultCaptureCallback(GX_FRAME_CALLBACK_PARAM *frame_callback);
  static void *DaemonThreadFunction(void *obj);

  static uint16_t camera_count_;
  GX_DEV_HANDLE device_{};
  int64_t color_filter_{};
  int64_t payload_size_{};
  unsigned char *raw_8_to_rgb_24_cache_{};
  unsigned char *raw_16_to_8_cache_{};
  static Registry<DHCamera> registry_;
};
}

#endif  // SRM_IC_2023_MODULES_CAMERA_DH_CAMERA_DH_H_
