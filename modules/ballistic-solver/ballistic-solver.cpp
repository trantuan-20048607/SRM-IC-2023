#include <glog/logging.h>
#include "coordinate/coordinate.h"
#include "ballistic-solver.h"

void ballistic_solver::AirResistanceModel::SetParam(double c, double p, double t, double d, double m) {
  constexpr double T = 273.15, atm = 1013.25;
  coeff_ = 0.5 * c * (1.293 * (p / atm) * (T / (T + t))) * (0.25 * M_PI * d * d) / m;
}

ballistic_solver::CVec ballistic_solver::AirResistanceModel::operator()(double t, CVec REF_IN v) const {
  return -coeff_ * v.norm() * v;
}

void ballistic_solver::GravityModel::SetParam(double p) {
  constexpr double deg2rad = M_PI / 180;
  double p_rad = p * deg2rad, s_1 = sin(p_rad), s_2 = sin(2 * p_rad);
  g_ = 9.78 * (1 + 0.0052884 * s_1 * s_1 - 0.0000059 * s_2 * s_2);
}

ballistic_solver::CVec ballistic_solver::GravityModel::operator()(double t, CVec REF_IN v) const {
  return {0, g_, 0};
}

void ballistic_solver::BallisticEquation::AddModel(std::shared_ptr<Model> REF_IN model) {
  models_.emplace_back(model);
}

ballistic_solver::CVec ballistic_solver::BallisticEquation::operator()(double t, CVec v) const {
  CVec result{};
  for (auto &&model : models_)
    result += (*model)(t, v);
  return result;
}

void ballistic_solver::BallisticSolver::AddModel(const std::shared_ptr<Model> &model) {
  solver_.f.AddModel(model);
}

void ballistic_solver::BallisticSolver::Initialize(double start_t, double end_t, CVec REF_IN start_x, size_t iter) {
  start_t_ = start_t;
  end_t_ = end_t;
  iter_ = iter;
  start_x_ = start_x;
}

void ballistic_solver::BallisticSolver::SetParam(CVec REF_IN start_v) {
  start_v_ = start_v;
  solver_.y = start_v_ + intrinsic_v_;
  solver_.h = (end_t_ - start_t_) / static_cast<double>(iter_);
  solver_.t = start_t_;
}

void ballistic_solver::BallisticSolver::Solve(
    std::function<bool(double t, CVec REF_IN v, CVec REF_IN x)> REF_IN solution_cond,
    std::function<bool(double t, CVec REF_IN v, CVec REF_IN x)> REF_IN iter_cond,
    std::vector<BallisticInfo> REF_OUT solutions) {
  solutions.clear();
  CVec current_x = start_x_ + solver_.y * solver_.h;
  for (size_t i = 0; i < iter_ && iter_cond(solver_.t, solver_.y, current_x); ++i) {
    solver_.forward();
    current_x += solver_.y * solver_.h;
    if (solution_cond(solver_.t, solver_.y, current_x))
      solutions.push_back({solver_.t, coordinate::CoordSolver::CTVecToSTVec(start_v_), solver_.y, current_x});
  }
}

ballistic_solver::BallisticInfo ballistic_solver::BallisticSolver::Solve(
    CVec REF_IN target_x, double start_v, CVec REF_IN intrinsic_v) {
  constexpr size_t max_iter = 16;
  intrinsic_v_ = intrinsic_v;
  size_t n = 0;
  double min_theta = -M_PI / 2, max_theta = M_PI / 2, mid_theta;
  double min_phi = -M_PI / 2, max_phi = M_PI / 2, mid_phi;
  std::vector<BallisticInfo> solutions;
  std::function<bool(double t, CVec REF_IN v, CVec REF_IN x)> solution_cond = [&](
      double t, CVec REF_IN v, CVec REF_IN x) -> bool {
    static double last_h = target_x.y();
    bool result = (target_x.y() - x.y()) * (target_x.y() - last_h) < 0;
    last_h = x.y();
    return result;
  }, iter_cond = [&](double t, CVec REF_IN v, CVec REF_IN x) -> bool {
    return solutions.size() < 2;
  };

  do {
    mid_theta = (min_theta + max_theta) / 2;
    mid_phi = (min_phi + max_phi) / 2;
    SetParam(coordinate::CoordSolver::STVecToCTVec({mid_phi, mid_theta, start_v}));
    Solve(solution_cond, iter_cond, solutions);
    // 是否达到目标高度（有解）
    // TODO 完善逻辑
  } while (n++ < max_iter);

  return {};
}