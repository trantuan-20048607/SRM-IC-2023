#ifndef SRM_IC_2023_MODULES_COMMON_BUFFER_H_
#define SRM_IC_2023_MODULES_COMMON_BUFFER_H_

#include <mutex>

template<typename T, size_t N>
class Buffer final {
 private:
  std::array<T, N> data_;
  size_t head_{}, tail_{};
  bool full_{};
  std::mutex lock_;

 public:
  Buffer() = default;
  ~Buffer() = default;

  inline void Push(T &&obj) {
    std::lock_guard<std::mutex> lock{lock_};
    data_[tail_] = std::forward<T>(obj);
    ++tail_ %= N;
    if (full_) ++head_ %= N;
    full_ = head_ == tail_;
  }

  inline bool Pop(T &obj) {
    std::lock_guard<std::mutex> lock{lock_};
    if (head_ == tail_ && !full_) return false;
    obj = std::move(data_[head_]);
    ++head_ %= N;
    full_ = false;
    return true;
  }
};

#endif  // SRM_IC_2023_MODULES_COMMON_BUFFER_H_
