#ifndef SRM_IC_2023_MODULES_VIDEO_SOURCE_BASE_VIDEO_SOURCE_BASE_H_
#define SRM_IC_2023_MODULES_VIDEO_SOURCE_BASE_VIDEO_SOURCE_BASE_H_

#include "common/factory.h"
#include "common/frame.h"
#include "common/attr-reader.h"

enable_factory(video_source, VideoSource)

namespace video_source {
class VideoSource {
 public:
  VideoSource() = default;
  virtual ~VideoSource() = default;

  attr_reader_ref(intrinsic_mat_, IntrinsicMat)
  attr_reader_ref(distortion_mat_, DistortionMat)

  virtual bool Initialize(const std::string &config_file) = 0;
  virtual bool GetFrame(Frame &frame) = 0;
  virtual void RegisterFrameCallback(FrameCallback callback, void *obj) = 0;
  virtual void UnregisterFrameCallback(FrameCallback callback) = 0;

 protected:
  cv::Mat intrinsic_mat_;
  cv::Mat distortion_mat_;
};
}

#endif  // SRM_IC_2023_MODULES_VIDEO_SOURCE_BASE_VIDEO_SOURCE_BASE_H_
