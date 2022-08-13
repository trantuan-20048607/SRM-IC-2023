#include "video-source-base.h"

video_source::VideoSource *video_source::CreateVideoSource(const std::string &type_name) {
  return video_source::Factory::Instance().Create(type_name);
}
