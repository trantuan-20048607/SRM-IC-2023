#ifndef SRM_IC_2023_MODULES_COORDINATE_COORDINATE_H_
#define SRM_IC_2023_MODULES_COORDINATE_COORDINATE_H_

#include <Eigen/Core>
#include <opencv2/core/mat.hpp>

namespace coordinate {
using Point2D = cv::Point2f;       // point on picture, with prefix p2d_
using Point3D = cv::Point3d;       // point in the world, with prefix p3d_
using RMat = Eigen::Matrix3d;      // rotation matrix, with prefix rm_
using TMat = cv::Mat;              // transformation matrix, with prefix tm_
using ExtTMat = Eigen::Matrix4d;   // extended transformation matrix (4x4), with prefix etm_
using CTVec = Eigen::Vector3d;     // cartesian translation vector (x, y, z -> x+: right, y+: below, z+: front), with prefix ctv_
using STVec = Eigen::Vector3d;     // spherical translation vector (y, p, d -> y+: right, p+: above, d+: any), with prefix stv_
using ExtCTVec = Eigen::Vector4d;  // extended translation vector (x, y, z, 1), with prefix ectv_
using EAngle = Eigen::Vector3d;    // euler angle (z, y, x -> roll+: right, yaw+: right, pitch+: above), with prefix ea_

struct PnPInfo {
  CTVec ctv_cam, ctv_world;
  STVec stv_cam, stv_world;
  RMat rm_cam;
  EAngle ea_cam;
};

class CoordSolver {
 private:
  TMat tm_intrinsic_, tm_distortion_;
  ExtTMat etm_ic_, etm_ci_;
  CTVec ctv_iw_;

 public:
  static EAngle RMatToEAngle(const RMat &rm);
  static RMat EAngleToRMat(const EAngle &ea);
  static STVec CTVecToSTVec(const CTVec &ctv);
  static CTVec STVecToCTVec(const STVec &stv);

  bool Initialize(const std::string &config_file, TMat tm_intrinsic, TMat tm_distortion);
  void SolvePnP(const std::vector<Point3D> &p3d_world,
                const std::vector<Point2D> &p2d_pic,
                PnPInfo &pnp_info,
                const RMat &rm_imu) const;
  CTVec CamToWorld(const CTVec &ctv_cam, const RMat &rm_imu) const;
  CTVec WorldToCam(const CTVec &ctv_world, const RMat &rm_imu) const;
  Point2D CamToPic(const CTVec &ctv_cam) const;
};
}

#endif  // SRM_IC_2023_MODULES_COORDINATE_COORDINATE_H_
