#ifndef SRM_IC_2023_MODULES_VIDEO_SOURCE_CAMERA_VIDEO_SOURCE_CAMERA_H_
#define SRM_IC_2023_MODULES_VIDEO_SOURCE_CAMERA_VIDEO_SOURCE_CAMERA_H_

#include "camera-base/camera-base.h"
#include "video-source-base/video-source-base.h"

namespace video_source::camera {
class CameraVideoSource final : public VideoSource {
 public:
  CameraVideoSource() = default;
  ~CameraVideoSource() final = default;

  bool Initialize(const std::string &config_file) final;
  bool GetFrame(Frame &frame) final;
  void RegisterFrameCallback(FrameCallback callback, void *obj) final;
  void UnregisterFrameCallback(FrameCallback callback) final;

 private:
  ::camera::Camera *camera_{};
  static Registry<CameraVideoSource> registry_;
};
}

#endif  // SRM_IC_2023_MODULES_VIDEO_SOURCE_CAMERA_VIDEO_SOURCE_CAMERA_H_
