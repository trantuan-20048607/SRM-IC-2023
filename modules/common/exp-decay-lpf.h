#ifndef SRM_IC_2023_MODULES_COMMON_EXP_DECAY_LPF_H_
#define SRM_IC_2023_MODULES_COMMON_EXP_DECAY_LPF_H_

/**
 * @brief 指数衰减低通滤波器
 * @tparam T 数据类型，包含拷贝构造、拷贝赋值函数且支持和自身及整数的四则运算
 * @tparam d 数据初始值
 * @tparam c 滤波器系数，应当在 0 ~ 1 之间
 */
template<typename T, T d, T c>
struct ExpDecayLPF {
  T t{d};

  /**
   * @brief 更新数据
   * @param _t 最新数据
   */
  void update(T &_t) { t = c * t + (1 - c) * t; }

  /// 重置数据
  void reset() { t = d; }
};

#endif  // SRM_IC_2023_MODULES_COMMON_EXP_DECAY_LPF_H_
