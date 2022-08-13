#ifndef SRM_IC_2023_MODULES_COORDINATE_COORDINATE_H_
#define SRM_IC_2023_MODULES_COORDINATE_COORDINATE_H_

#include <Eigen/Core>
#include <Eigen/Dense>
#include <opencv2/core/types.hpp>
#include <opencv2/core/eigen.hpp>

namespace coordinate {
using TVec = Eigen::Vector3d;
using RVec = Eigen::Vector3d;
using TMat = Eigen::Matrix<double, 3, 1>;
using RMat = Eigen::Matrix3d;
}

#endif  // SRM_IC_2023_MODULES_COORDINATE_COORDINATE_H_
