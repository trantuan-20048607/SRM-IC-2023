#include <glog/logging.h>
#include "armor.h"

Armor::Armor(const std::array<cv::Point2f, 4> &corners,
             const coordinate::CoordSolver &coord_solver,
             const coordinate::EAngle &euler_angle,
             ArmorSize size) : size_(size) {
  for (auto i = 0; i < 4; ++i) corners_[i] = corners[i];
  for (auto &&corner : corners_) center_ += corner;
  center_ /= 4;
  std::vector<coordinate::Point3D> p3d_world;
  switch (size_) {
    case ArmorSize::BIG: {
      p3d_world = {
          {-0.1125, 0.027, 0},
          {-0.1125, -0.027, 0},
          {0.1125, -0.027, 0},
          {0.1125, 0.027, 0}};
      break;
    }
    default: LOG(ERROR) << "Unknown armor size type. Fallback to small armor.";
    case ArmorSize::SMALL: {
      p3d_world = {
          {-0.066, 0.027, 0},
          {-0.066, -0.027, 0},
          {0.066, -0.027, 0},
          {0.066, 0.027, 0}};
      break;
    }
  }
  coord_solver.SolvePnP(p3d_world, {corners_.begin(), corners_.end()}, pnp_info_,
                        coordinate::CoordSolver::EAngleToRMat(euler_angle));
}
