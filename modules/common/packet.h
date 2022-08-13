#ifndef SRM_IC_2023_MODULES_COMMON_PACKET_H_
#define SRM_IC_2023_MODULES_COMMON_PACKET_H_

#include <ostream>

struct ReceivePacket {
};

struct SendPacket {
};

inline std::ostream &operator<<(std::ostream &str, const ReceivePacket &packet) {
  return str;
}

inline std::ostream &operator<<(std::ostream &str, const SendPacket &packet) {
  return str;
}

#endif  // SRM_IC_2023_MODULES_COMMON_PACKET_H_
