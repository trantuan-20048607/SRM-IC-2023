#ifndef SRM_IC_2023_MODULES_SERIAL_SERIAL_H_
#define SRM_IC_2023_MODULES_SERIAL_SERIAL_H_

#include <mutex>
#include <thread>
#include <glog/logging.h>
#include "common/packet.h"

namespace serial {
class Serial final {
 public:
  Serial() = default;
  ~Serial();
  bool Open();
  void Close();
  bool ReadData(ReceivePacket &data);
  bool WriteData(const SendPacket &data);

 private:
  bool OpenPort();
  void ClosePort();
  bool SerialSend();
  bool SerialReceive();

  std::string serial_port_;
  int serial_fd_{};
  bool com_flag_{};
  std::timed_mutex send_data_lock_, receive_data_lock_;
  SendPacket send_data_{};
  ReceivePacket receive_data_{};
};
}

#endif  // SRM_IC_2023_MODULES_SERIAL_SERIAL_H_
