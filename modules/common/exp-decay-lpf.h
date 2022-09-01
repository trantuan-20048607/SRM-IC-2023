#ifndef SRM_IC_2023_MODULES_COMMON_EXP_DECAY_LPF_H_
#define SRM_IC_2023_MODULES_COMMON_EXP_DECAY_LPF_H_

/**
 * @brief 指数衰减低通滤波器
 * @tparam T 数据类型，包含拷贝构造、拷贝赋值函数且支持和自身和 double 的四则运算
 * @tparam h 历史权重
 * @tparam c 更新权重
 */
template<typename T, size_t h, size_t c>
struct ExpDecayLPF {
  T data;

  /**
   * @brief 更新数据
   * @param _data 最新数据
   */
  void update(T &_data) {
    data = (h * data + c * _data) / (h + c);
  }
};

#endif  // SRM_IC_2023_MODULES_COMMON_EXP_DECAY_LPF_H_
