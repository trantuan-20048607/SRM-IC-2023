#ifndef SRM_IC_2023_MODULES_COMMON_FRAME_H_
#define SRM_IC_2023_MODULES_COMMON_FRAME_H_

#include <functional>
#include <opencv2/core/mat.hpp>
#include "packet.h"

struct Frame {
  cv::Mat image;
  ReceivePacket receive_packet;
  uint64_t time_stamp;
};

using FrameCallback = std::function<void(void *, Frame &)> *;

#endif  // SRM_IC_2023_MODULES_COMMON_FRAME_H_
