#ifndef SRM_IC_2023_MODULES_COMMON_ARMOR_H_
#define SRM_IC_2023_MODULES_COMMON_ARMOR_H_

#include <opencv2/core/types.hpp>
#include "attr-reader.h"
#include "coordinate/coordinate.h"

class Armor final {
  enum class ArmorSize { BIG, SMALL };

 public:
  Armor(const std::array<cv::Point2f, 4> &corners,
        const coordinate::CoordSolver &coord_solver,
        const coordinate::EAngle &euler_angle,
        ArmorSize size);

  attr_reader_ref(corners_, Corners)
  attr_reader_ref(center_, Center)
  attr_reader_ref(pnp_info_.ctv_cam, CTVecCam)
  attr_reader_ref(pnp_info_.stv_cam, STVecCam)
  attr_reader_ref(pnp_info_.ctv_world, CTVecWorld)
  attr_reader_ref(pnp_info_.stv_world, STVecWorld)
  attr_reader_ref(pnp_info_.ea_cam, EAngleCam)
  attr_reader_val(size_, Size)

 private:
  std::array<cv::Point2f, 4> corners_;
  coordinate::PnPInfo pnp_info_;
  cv::Point2f center_;
  ArmorSize size_;
};

#endif  // SRM_IC_2023_MODULES_COMMON_ARMOR_H_
