#include "camera-base.h"

void camera::Camera::RegisterFrameCallback(FrameCallback callback, void *obj) {
  callback_list_.emplace_back(callback, obj);
}

void camera::Camera::UnregisterFrameCallback(FrameCallback callback) {
  callback_list_.erase(std::remove_if(
      callback_list_.begin(), callback_list_.end(),
      [callback](auto p) { return p.first == callback; }), callback_list_.end());
}

camera::Camera *camera::CreateCamera(const std::string &type_name) {
  return camera::Factory::Instance().Create(type_name);
}
